/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Simple_cloud.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/30 13:58:09 by cledant           #+#    #+#             */
/*   Updated: 2017/09/21 10:51:29 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Simple_cloud.hpp"

Simple_cloud::Simple_cloud(size_t nb_particle, cl::Context const *context,
	glm::vec3 const &pos, Shader const *shader, cl::CommandQueue const *cq,
	std::vector<cl::Kernel const *> const &vec_random, cl::Kernel const *gravity,
	glm::mat4 const *perspec_mult_view, float refresh_tick) :
	_shader(shader), _cl_cq(cq), _cl_vec_random_kernel(vec_random),
	_cl_kernel_gravity(gravity), _perspec_mult_view(perspec_mult_view),
	_generate_random(true), _update_gravity(false), _pos(pos), _gl_vbo(0),
	_gl_vao(0), _refresh_tick(refresh_tick), _cur_random(0),
	_grav_ctrl_type(MOUSE_CLICK)
{
	if (nb_particle == 0)
		throw Simple_cloud::Simple_cloudFailException();
	this->_nb_particle = nb_particle;
	try
	{
		if (this->_cl_vec_random_kernel.size() == 0)
			throw Simple_cloudFailException();
		this->_cur_random = 0;
		this->_cl_kernel_random = (this->_cl_vec_random_kernel)[0];
		this->_gl_vbo = oGL_module::oGL_create_vbo(nb_particle * sizeof(t_particle),
				NULL);
		this->_gl_vao = oGL_module::oGL_create_vao();
		oGL_module::oGL_set_vao_parameters(this->_gl_vao, this->_gl_vbo, 0,
			3, sizeof(t_particle), 0);
		oCL_module::oCL_create_cl_vbo(this->_gl_vbo, *context, this->_cl_vbo);
		this->_center_mass = 1.f * std::pow(10.0f, 24);
		this->_particle_mass = 1.0f;
		this->_grav_mult = 1.0f;
		this->_color = 0x0000F0F0;
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
		this->_generate_random = true;
		this->_pos = {0.0f, 0.0f, 0.0f};
		this->_update_gravity = false;
		this->_cur_random = (this->_cur_random + 1) %
			this->_cl_vec_random_kernel.size();
		this->_cl_kernel_random = this->_cl_vec_random_kernel[this->_cur_random];
		this->_grav_mult = 1.0f;
		return (true);	
	}
	else if (input.p_key[GLFW_KEY_T] == PRESSED && input_timer > 0.5f)
	{
		this->_grav_ctrl_type =
			static_cast<t_gravity_control>((this->_grav_ctrl_type + 1) % 2);
		return (true);
	}
	return (false);
}

/*
 * Axis[0] should be Front vector
 * Axis[1] should be Right vector
 * Axis[2] should be Up vector
*/

bool				Simple_cloud::update_mouse_interaction(Input const &input,
						Window const &win, glm::vec3 const &origin,
						std::vector<glm::vec3 const *> const &axes,
						float input_timer)
{
	float			ratio_w = 0.0f;
	float			ratio_h = 0.0f;
	glm::vec3		final_pos;
	float			offset_z = 10.0f;

	static_cast<void>(input_timer);
	if (((input.p_mouse[GLFW_MOUSE_BUTTON_1] == PRESSED &&
			this->_grav_ctrl_type == MOUSE_CLICK) || (this->_grav_ctrl_type ==
			MOUSE_FOLLOW)) && input.mouse_exclusive == false)
	{
		ratio_w = input.last_pos_x  /
			static_cast<float>(win.cur_win_w);
		ratio_h = (static_cast<float>(win.cur_win_h) - input.last_pos_y) /
			static_cast<float>(win.cur_win_h);
		final_pos.x = origin.x + ((ratio_w * axes[2]->x + ratio_h * axes[1]->x +
			1.0f * axes[0]->x) - 0.5) * 8.0f;
		final_pos.y = origin.y + ((ratio_w * axes[2]->y + ratio_h * axes[1]->y +
			1.0f * axes[0]->y) - 0.5) * 8.0f;
		final_pos.z = origin.z + (ratio_w * axes[2]->z + ratio_h * axes[1]->z +
			offset_z * axes[0]->z);
		this->_pos = final_pos;
		std::cout << "=============" << std::endl;
		std::cout << "ratio_w = " << ratio_w << std::endl;
		std::cout << "ratio_h = " << ratio_h << std::endl;
		std::cout << "cur_win_w = " << win.cur_win_w << std::endl;
		std::cout << "cur_win_h = " << win.cur_win_h << std::endl;
		std::cout << "last_pos_x = " << input.last_pos_x << std::endl;
		std::cout << "last_pos_y = " << input.last_pos_y << std::endl;
		std::cout << "=============" << std::endl;
		std::cout << "=============" << std::endl;
		std::cout << "up_x = " << axes[2]->x << std::endl;
		std::cout << "up_y = " << axes[2]->y << std::endl;	
		std::cout << "up_z = " << axes[2]->z << std::endl;
		std::cout << "=============" << std::endl;
		std::cout << "=============" << std::endl;
		std::cout << "front_x = " << axes[0]->x << std::endl;
		std::cout << "front_y = " << axes[0]->y << std::endl;	
		std::cout << "front_z = " << axes[0]->z << std::endl;
		std::cout << "=============" << std::endl;
		std::cout << "=============" << std::endl;
		std::cout << "right_x = " << axes[1]->x << std::endl;
		std::cout << "right_y = " << axes[1]->y << std::endl;	
		std::cout << "right_z = " << axes[1]->z << std::endl;
		std::cout << "=============" << std::endl;
		std::cout << "=============" << std::endl;
		std::cout << "new_pos_x = " << final_pos.x << std::endl;
		std::cout << "new_pos_y = " << final_pos.y << std::endl;	
		std::cout << "new_pos_z = " << final_pos.z << std::endl;
		std::cout << "=============" << std::endl;
		std::cout << "=============" << std::endl;
		std::cout << "ori_pos_x = " << origin.x << std::endl;
		std::cout << "ori_pos_y = " << origin.y << std::endl;	
		std::cout << "ori_pos_z = " << origin.z << std::endl;
		std::cout << "=============" << std::endl;
	}
	return (false);
}

void				Simple_cloud::draw(void)
{
	GLint		mat_total_id;
	GLint		cloud_color_id;

	if (this->_shader == nullptr || this->_perspec_mult_view == nullptr ||
		this->_cl_cq == nullptr || this->_cl_kernel_random == nullptr ||
		this->_cl_kernel_gravity == nullptr ||
		oGL_module::oGL_getUniformID("mat_total", this->_shader->getShaderProgram(),
		&mat_total_id) == false || oGL_module::oGL_getUniformID("cloud_color",
		this->_shader->getShaderProgram(), &cloud_color_id) == false)
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
	float				min = -2.0f;
	float				max = 2.0f;
	unsigned int		ran_x[2];
	unsigned int		ran_y[2];
	unsigned int		ran_z[2];

	this->_generate_random_uint2(&ran_x);
	this->_generate_random_uint2(&ran_y);
	this->_generate_random_uint2(&ran_z);
	const_cast<cl::Kernel *>(this->_cl_kernel_random)->setArg(0, this->_cl_vbo);
	const_cast<cl::Kernel *>(this->_cl_kernel_random)->setArg(1, min);
	const_cast<cl::Kernel *>(this->_cl_kernel_random)->setArg(2, max);
	const_cast<cl::Kernel *>(this->_cl_kernel_random)->setArg(3, ran_x);
	const_cast<cl::Kernel *>(this->_cl_kernel_random)->setArg(4, ran_y);
	const_cast<cl::Kernel *>(this->_cl_kernel_random)->setArg(5, ran_z);
	const_cast<cl::Kernel *>(this->_cl_kernel_random)->setArg(6, this->_pos);
	this->_cl_cq->finish();
}

void				Simple_cloud::_set_gravity_kernel_args(void)
{
	const_cast<cl::Kernel *>(this->_cl_kernel_gravity)->setArg(0, this->_cl_vbo);
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

Simple_cloud::Simple_cloudFailException::Simple_cloudFailException(void)
{
	this->_msg = "Simple Cloud : Something failed !";
}

Simple_cloud::Simple_cloudFailException::~Simple_cloudFailException(void) throw()
{
}
