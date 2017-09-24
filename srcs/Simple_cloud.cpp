/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Simple_cloud.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/30 13:58:09 by cledant           #+#    #+#             */
/*   Updated: 2017/09/24 11:01:17 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Simple_cloud.hpp"

Simple_cloud::Simple_cloud(Simple_cloud::Params const &init) :
	_shader(init.shader), _cl_cq(init.cq), _cl_vec_random_kernel(init.vec_random),
	_cl_kernel_gravity(init.gravity), _cl_kernel_lifetime(init.lifetime),
	_perspec_mult_view(init.perspec_mult_view), _nb_particle(init.nb_particle),
	_pos(init.gravity_pos), _emitter_pos(init.emitter_pos), _gl_vbo(0), _gl_vao(0),
	_refresh_tick(init.refresh_tick), _cur_random(0),
	_grav_ctrl_type(init.grav_ctrl_type), _click_type(init.click_type),
	_color(init.color), _grav_mult(1.0f), _min_random(init.min_random),
	_max_random(init.max_random), _min_lifetime(init.min_lifetime),
	_max_lifetime(init.max_lifetime), _generate_random(true),
	_update_gravity(init.update_gravity), _update_lifetime(init.update_lifetime)
{
	if (this->_nb_particle == 0)
		throw Simple_cloud::Simple_cloudFailException();
	try
	{
		if (this->_cl_vec_random_kernel.size() == 0)
			throw Simple_cloudFailException();
		this->_cl_kernel_random = (this->_cl_vec_random_kernel)[0];
		this->_gl_vbo = oGL_module::oGL_create_vbo(this->_nb_particle *
				sizeof(t_particle), NULL);
		this->_gl_vao = oGL_module::oGL_create_vao();
		oGL_module::oGL_set_vao_parameters(this->_gl_vao, this->_gl_vbo, 0,
			3, sizeof(t_particle), 0);
		oCL_module::oCL_create_cl_vbo(this->_gl_vbo, *(init.context), this->_cl_vbo);
		this->_center_mass = 1.f * std::pow(10.0f, 24);
		this->_particle_mass = 1.0f;
		this->update(0.0f);
	}
	catch (std::exception &e)
	{
		oGL_module::oGL_delete_vbo(this->_gl_vbo);
		oGL_module::oGL_delete_vbo(this->_gl_vao);
		throw Simple_cloud::Simple_cloudFailException();
	}
}

Simple_cloud::~Simple_cloud(void)
{
	this->_cl_cq->finish();
	oGL_module::oGL_delete_vbo(this->_gl_vbo);
	oGL_module::oGL_delete_vao(this->_gl_vao);
}

Simple_cloud::Simple_cloud(Simple_cloud const &src)
{
	static_cast<void>(src);
}

Simple_cloud		&Simple_cloud::operator=(Simple_cloud const &rhs)
{
	static_cast<void>(rhs);
	return (*this);
}

Simple_cloud::Params::Params(void)
{
	this->nb_particle = 1000000;
	this->context = nullptr;
	this->gravity_pos = {0.0f, 0.0f, 0.0f};
	this->emitter_pos = {0.0f, 0.0f, 0.0f};
	this->shader = nullptr;
	this->cq = nullptr;
	this->gravity = nullptr;
	this->lifetime = nullptr;
	this->perspec_mult_view = nullptr;
	this->refresh_tick = 0.01667f;
	this->min_random = -2.0f;
	this->max_random = 2.0f;
	this->min_lifetime = 160.0f;
	this->max_lifetime = 300.0f;
	this->color = 0x0000F0F0;
	this->update_gravity = false;
	this->update_lifetime = false;
	this->grav_ctrl_type = MOUSE_CLICK;
	this->click_type = GRAVITY_POS;
}

Simple_cloud::Params::~Params(void)
{
}

void				Simple_cloud::update(float time)
{
	static_cast<void>(time);

	if (this->_shader == nullptr || this->_perspec_mult_view == nullptr ||
		this->_cl_cq == nullptr || this->_cl_kernel_random == nullptr ||
		this->_cl_kernel_gravity == nullptr)
	{
		std::cout << "Warning : Can't update Simple_cloud" << std::endl;
		return ;
	}
	this->_convert_color_to_float_color();
	this->_total = *(this->_perspec_mult_view);
}

bool				Simple_cloud::update_keyboard_interaction(Input const &input,
						float input_timer)
{
	if (input.mouse_exclusive == true)
		this->_grav_ctrl_type = MOUSE_CLICK;
	if (input.p_key[GLFW_KEY_P] == PRESSED && input_timer > 0.5f)
	{
		this->_update_gravity = (this->_update_gravity == true) ? false : true;
		return (true);
	}
	else if (input.p_key[GLFW_KEY_KP_SUBTRACT] == PRESSED && input_timer > 0.1f)
	{
		this->_grav_mult -= 0.05;
		if (this->_grav_mult <= 0.25f)
			this->_grav_mult = 0.25f;
		return (true);
	}
	else if (input.p_key[GLFW_KEY_KP_ADD] == PRESSED && input_timer > 0.1f)
	{
		this->_grav_mult += 0.05;
		if (this->_grav_mult >= 2.5f)
			this->_grav_mult = 2.5f;
		return (true);
	}
	else if (input.p_key[GLFW_KEY_EQUAL] == PRESSED && input_timer > 0.05f)
	{
		this->_right_shift_color();
		this->_convert_color_to_float_color();
		return (true);
	}
	else if (input.p_key[GLFW_KEY_MINUS] == PRESSED && input_timer > 0.05f)
	{
		this->_left_shift_color();
		this->_convert_color_to_float_color();
		return (true);
	}
	else if (input.p_key[GLFW_KEY_R] == PRESSED && input_timer > 0.5f)
	{
		this->_reset_and_switch_type();
		return (true);	
	}
	else if (input.p_key[GLFW_KEY_T] == PRESSED && input_timer > 0.5f)
	{
		this->_switch_gravity_mode();
		return (true);
	}
	else if (input.p_key[GLFW_KEY_L] == PRESSED && input_timer > 0.5f)
	{
		this->_switch_lifetime_mode();
		return (true);
	}
	else if (input.p_key[GLFW_KEY_Y] == PRESSED && input_timer > 0.5f)
	{
		this->_switch_click_mode();
		return (true);
	}
	return (false);
}

bool				Simple_cloud::update_mouse_interaction(Input const &input,
						Window const &win, glm::vec3 const &origin,
						std::vector<glm::vec3 const *> const &axes,
						float input_timer)
{
	static_cast<void>(input_timer);
	this->_compute_mouse_3d_pos(input, win, origin, axes);
	if (((input.p_mouse[GLFW_MOUSE_BUTTON_1] == PRESSED &&
			this->_grav_ctrl_type == MOUSE_CLICK) || (this->_grav_ctrl_type ==
			MOUSE_FOLLOW)) && input.mouse_exclusive == false &&
			this->_click_type == GRAVITY_POS)
		this->_pos = this->_mouse_3d_pos;
	if (((input.p_mouse[GLFW_MOUSE_BUTTON_1] == PRESSED &&
			this->_grav_ctrl_type == MOUSE_CLICK) || (this->_grav_ctrl_type ==
			MOUSE_FOLLOW)) && input.mouse_exclusive == false &&
			this->_click_type == EMITTER_POS)
		this->_emitter_pos = this->_mouse_3d_pos;
	return (false);
}

void				Simple_cloud::draw(void)
{
	GLint		mat_total_id;
	GLint		cloud_color_id;
	GLint		mouse_3d_pos_id;

	if (this->_shader == nullptr || this->_perspec_mult_view == nullptr ||
		this->_cl_cq == nullptr || this->_cl_kernel_random == nullptr ||
		this->_cl_kernel_gravity == nullptr || this->_cl_vbo.size() == 0 ||
		this->_cl_kernel_lifetime == nullptr ||
		oGL_module::oGL_getUniformID("mat_total", this->_shader->getShaderProgram(),
		&mat_total_id) == false || oGL_module::oGL_getUniformID("cloud_color",
		this->_shader->getShaderProgram(), &cloud_color_id) == false ||
		oGL_module::oGL_getUniformID("mouse_3d_pos",
		this->_shader->getShaderProgram(), &mouse_3d_pos_id) == false)
	{
		std::cout << "Warning : Can't draw Simple_cloud" << std::endl;
		return ;
	}
	if (this->_generate_random == true)
	{
		this->_set_random_kernel_args();
		oCL_module::oCL_run_kernel_oGL_buffer(this->_cl_vbo,
			const_cast<cl::Kernel &>(*(this->_cl_kernel_random)),
			const_cast<cl::CommandQueue &>(*(this->_cl_cq)), this->_nb_particle);
		this->_generate_random = false;
	}
	if (this->_update_lifetime == true && this->_update_gravity == true)
	{
		this->_set_lifetime_kernel_args();
		oCL_module::oCL_run_kernel_oGL_buffer(this->_cl_vbo,
			const_cast<cl::Kernel &>(*(this->_cl_kernel_lifetime)),
			const_cast<cl::CommandQueue &>(*(this->_cl_cq)), this->_nb_particle);
		this->_generate_random = false;	
	}
	if (this->_update_gravity == true)
	{
		this->_set_gravity_kernel_args();
		oCL_module::oCL_run_kernel_oGL_buffer(this->_cl_vbo,
			const_cast<cl::Kernel &>(*(this->_cl_kernel_gravity)),
			const_cast<cl::CommandQueue &>(*(this->_cl_cq)), this->_nb_particle);
	}
	this->_shader->use();
	this->_shader->setMat4(mat_total_id, this->_total);
	this->_shader->setVec3(cloud_color_id, this->_gl_color);
	this->_shader->setVec3(mouse_3d_pos_id, this->_mouse_3d_pos);
	oGL_module::oGL_draw_points(this->_gl_vao, this->_nb_particle);
}

void				Simple_cloud::setPosition(glm::vec3 const &pos)
{
	this->_pos = pos;
}

glm::mat4 const 	&Simple_cloud::getTotalMatrix(void) const
{
	return (this->_total);
}

void				Simple_cloud::_generate_random_uint2(unsigned int (*random)[2])
{
	std::mt19937_64									gen(this->_rd());
	std::uniform_int_distribution<unsigned int>
		dis(0, std::numeric_limits<unsigned int>::max());

	unsigned int	other = dis(gen);

	(*random)[0] = ((dis(gen) >> 3) * 1468516) ^ (other << 9);
	(*random)[1] = ((dis(gen) << 7) * 874161) ^ (other >> 7);
}

void				Simple_cloud::_set_random_kernel_args(void)
{
	float				minmax[2] = {this->_min_random, this->_max_random};
	float				lifetime[2] = {this->_min_lifetime, this->_max_lifetime};
	unsigned int		ran_x[2];
	unsigned int		ran_y[2];
	unsigned int		ran_z[2];

	this->_cl_cq->finish();
	this->_generate_random_uint2(&ran_x);
	this->_generate_random_uint2(&ran_y);
	this->_generate_random_uint2(&ran_z);
	const_cast<cl::Kernel *>(this->_cl_kernel_random)->setArg(0, (this->_cl_vbo)[0]);
	const_cast<cl::Kernel *>(this->_cl_kernel_random)->setArg(1, minmax);
	const_cast<cl::Kernel *>(this->_cl_kernel_random)->setArg(2, lifetime);
	const_cast<cl::Kernel *>(this->_cl_kernel_random)->setArg(3, ran_x);
	const_cast<cl::Kernel *>(this->_cl_kernel_random)->setArg(4, ran_y);
	const_cast<cl::Kernel *>(this->_cl_kernel_random)->setArg(5, ran_z);
	const_cast<cl::Kernel *>(this->_cl_kernel_random)->setArg(6, this->_pos);
	this->_cl_cq->finish();
}

void				Simple_cloud::_set_lifetime_kernel_args(void)
{
	float				minmax[2] = {this->_min_random / 10.0f,
										this->_max_random / 10.0f};
	float				lifetime[2] = {this->_min_lifetime, this->_max_lifetime};
	unsigned int		ran_x[2];
	unsigned int		ran_y[2];
	unsigned int		ran_z[2];

	this->_cl_cq->finish();
	this->_generate_random_uint2(&ran_x);
	this->_generate_random_uint2(&ran_y);
	this->_generate_random_uint2(&ran_z);
	const_cast<cl::Kernel *>(this->_cl_kernel_lifetime)->setArg(0,
		(this->_cl_vbo)[0]);
	const_cast<cl::Kernel *>(this->_cl_kernel_lifetime)->setArg(1, minmax);
	const_cast<cl::Kernel *>(this->_cl_kernel_lifetime)->setArg(2, lifetime);
	const_cast<cl::Kernel *>(this->_cl_kernel_lifetime)->setArg(3, ran_x);
	const_cast<cl::Kernel *>(this->_cl_kernel_lifetime)->setArg(4, ran_y);
	const_cast<cl::Kernel *>(this->_cl_kernel_lifetime)->setArg(5, ran_z);
	const_cast<cl::Kernel *>(this->_cl_kernel_lifetime)->setArg(6,
		this->_emitter_pos);
	const_cast<cl::Kernel *>(this->_cl_kernel_lifetime)->setArg(7,
		this->_refresh_tick * 10.0f);
	this->_cl_cq->finish();
}

void				Simple_cloud::_set_gravity_kernel_args(void)
{
	this->_cl_cq->finish();
	const_cast<cl::Kernel *>(this->_cl_kernel_gravity)->setArg(0,
		(this->_cl_vbo)[0]);
	const_cast<cl::Kernel *>(this->_cl_kernel_gravity)->setArg(1, this->_pos);
	const_cast<cl::Kernel *>(this->_cl_kernel_gravity)->setArg(2,
		this->_refresh_tick);
	const_cast<cl::Kernel *>(this->_cl_kernel_gravity)->setArg(3,
		this->_particle_mass);
	const_cast<cl::Kernel *>(this->_cl_kernel_gravity)->setArg(4,
		this->_center_mass);
	const_cast<cl::Kernel *>(this->_cl_kernel_gravity)->setArg(5,
		this->_grav_mult);
	this->_cl_cq->finish();
}

void				Simple_cloud::_right_shift_color(void)
{
	unsigned int 		mask = 0x1;
	unsigned int		add = 0x80000000;
	unsigned int		bit;

	bit = mask & this->_color;
	this->_color = this->_color >> 1;
	if (bit == mask)
		this->_color = this->_color | add;
}

void				Simple_cloud::_left_shift_color(void)
{
	unsigned int 		add = 0x1;
	unsigned int		mask = 0x80000000;
	unsigned int		bit;

	bit = mask & this->_color;
	this->_color = this->_color << 1;
	if (bit == mask)
		this->_color = this->_color | add;
}

void				Simple_cloud::_convert_color_to_float_color(void)
{
	t_rgba		*cast = reinterpret_cast<t_rgba *>(&(this->_color));

	this->_gl_color.x = 1.0f / static_cast<float>(cast->r);
	this->_gl_color.y = 1.0f / static_cast<float>(cast->g);
	this->_gl_color.z = 1.0f / static_cast<float>(cast->b);
}

/*
 * Axis[0] should be Front vector
 * Axis[1] should be Right vector
 * Axis[2] should be Up vector
 * 8.0f is a magic number
*/

void				Simple_cloud::_compute_mouse_3d_pos(Input const &input,
						Window const &win, glm::vec3 const &origin,
						std::vector<glm::vec3 const *> const &axes)
{
 	float		win_x = static_cast<float>(win.cur_win_w);
	float		win_y = static_cast<float>(win.cur_win_h);
	float		pitch_x = 1.0f / win_x;
	float		pitch_y = 1.0f / win_y;
	float 		pos_x = input.last_pos_x;
	float 		pos_y = (win_y - input.last_pos_y);
	float		ratio_x = win_x / win_y;
	float		ratio_y = win_y / win_x;
	float 		mx = (pos_x - (win_x * 0.5)) * pitch_x * 8.0f * ratio_x;
	float 		my = (pos_y - (win_y * 0.5)) * pitch_y * 8.0f * ratio_y;
	glm::vec3 	dx = *(axes[2]) * mx;
	glm::vec3 	dy = *(axes[1]) * my;
		
	this->_mouse_3d_pos = *(axes[0]) * 10.0f + dx + dy + origin;
}

void				Simple_cloud::_switch_gravity_mode(void)
{
	this->_grav_ctrl_type =
			static_cast<t_gravity_control>((this->_grav_ctrl_type + 1) % 2);
}

void				Simple_cloud::_switch_lifetime_mode(void)
{
	this->_update_lifetime = (this->_update_lifetime == true) ? false : true;
}

void				Simple_cloud::_switch_click_mode(void)
{
	this->_click_type =
			static_cast<t_click_type>((this->_click_type + 1) % 2);
}

void				Simple_cloud::_reset_and_switch_type(void)
{
	this->_generate_random = true;
	this->_update_lifetime = false;
	this->_pos = {0.0f, 0.0f, 0.0f};
	this->_emitter_pos = {0.0f, 0.0f, 0.0f};
	this->_update_gravity = false;
	this->_cur_random = (this->_cur_random + 1) % this->_cl_vec_random_kernel.size();
	this->_cl_kernel_random = this->_cl_vec_random_kernel[this->_cur_random];
	this->_grav_mult = 1.0f;
}

Simple_cloud::Simple_cloudFailException::Simple_cloudFailException(void)
{
	this->_msg = "Simple Cloud : Something failed !";
}

Simple_cloud::Simple_cloudFailException::~Simple_cloudFailException(void) throw()
{
}
