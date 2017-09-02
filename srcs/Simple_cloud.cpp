/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Simple_cloud.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/30 13:58:09 by cledant           #+#    #+#             */
/*   Updated: 2017/09/02 12:57:01 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Simple_cloud.hpp"

Simple_cloud::Simple_cloud(void)
{
	this->_nb_particle = 0;
	this->_cl_context = nullptr;
	this->_cl_cc = nullptr;
	this->_cl_program = nullptr;
}

Simple_cloud::Simple_cloud(size_t nb_particle, cl::Context const *cl_context,
				cl::CommandQueue const *cl_cc, cl::Program const *cl_prog)
{
	this->_nb_particle = nb_particle;
	this->_cl_context = cl_context;
	this->_cl_cc = cl_cc;
	this->_cl_program = cl_prog;
	try
	{
		this->_gl_vbo = oGL_module::oGL_create_vbo(nb_particle *
			sizeof(GLfloat) * 4);
		oCL_module::oCL_create_cl_vbo(this->_gl_vbo, *cl_context, this->_cl_vbo);
		this->set_kernel_args();
	}
	catch (std::exception &e)
	{
		oGL_module::oGL_delete_vbo(this->_gl_vbo);
		throw Simple_cloud::Simple_cloudFailException();
	}
}

Simple_cloud::~Simple_cloud(void)
{
	if (this->_nb_particle != 0)
		oGL_module::oGL_delete_vbo(this->_gl_vbo);
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

GLuint				Simple_cloud::get_gl_vbo(void) const
{
	return (this->_gl_vbo);
}

cl::BufferGL const	&Simple_cloud::get_cl_vbo(void) const
{
	return (this->_cl_vbo);
}

cl::Context const	*Simple_cloud::get_cl_context(void) const
{
	return (this->_cl_context);
}

size_t				Simple_cloud::get_nb_particle(void) const
{
	return (this->_nb_particle);
}

Simple_cloud::Simple_cloudFailException::Simple_cloudFailException(void)
{
	this->_msg = "Simple Cloud : Something failed !";
}

Simple_cloud::Simple_cloudFailException::~Simple_cloudFailException(void) throw()
{
}

std::string		Simple_cloud::_kernel_name = "create_random_sphere";
std::string		Simple_cloud::_kernel_path = "./kernel/create_random_sphere.cl"
