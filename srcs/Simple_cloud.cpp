/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Simple_cloud.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/30 13:58:09 by cledant           #+#    #+#             */
/*   Updated: 2017/09/18 13:25:33 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Simple_cloud.hpp"

Simple_cloud::Simple_cloud(size_t nb_particle, cl::Context const *context,
	glm::vec3 const &pos, Shader const *shader, cl::CommandQueue const *cq,
	cl::Kernel const *random, cl::Kernel const *gravity,
	glm::mat4 const *perspec_mult_view, float refresh_tick) : _shader(shader),
	_cl_cq(cq), _cl_kernel_random(random), _cl_kernel_gravity(gravity),
	_perspec_mult_view(perspec_mult_view), _generate_random(true),
	_update_gravity(true), _pos(pos), _gl_vbo(0), _gl_vao(0),
	_refresh_tick(refresh_tick)
{
	if (nb_particle == 0)
		throw Simple_cloud::Simple_cloudFailException();
	this->_nb_particle = nb_particle;
	try
	{
		this->_gl_vbo = oGL_module::oGL_create_vbo(nb_particle * sizeof(t_particle),
				NULL);
		this->_gl_vao = oGL_module::oGL_create_vao();
		oGL_module::oGL_set_vao_parameters(this->_gl_vao, this->_gl_vbo, 0,
			3, sizeof(t_particle), 0);
		oCL_module::oCL_create_cl_vbo(this->_gl_vbo, *context, this->_cl_vbo);
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
	this->_total = *(this->_perspec_mult_view);
}

bool				Simple_cloud::update_interaction(Input const &input)
{
	static_cast<void>(input);
	return (false);
}

void				Simple_cloud::draw(void)
{
	GLint		uniform_id;

	if (this->_shader == nullptr || this->_perspec_mult_view == nullptr ||
		this->_cl_cq == nullptr || this->_cl_kernel_random == nullptr ||
		this->_cl_kernel_gravity == nullptr ||
		oGL_module::oGL_getUniformID("mat_total", this->_shader->getShaderProgram(),
		&uniform_id) == false)
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
	this->_shader->setMat4(uniform_id, this->_total);
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
	this->_cl_cq->finish();
}

Simple_cloud::Simple_cloudFailException::Simple_cloudFailException(void)
{
	this->_msg = "Simple Cloud : Something failed !";
}

Simple_cloud::Simple_cloudFailException::~Simple_cloudFailException(void) throw()
{
}
