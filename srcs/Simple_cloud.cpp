/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Simple_cloud.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/30 13:58:09 by cledant           #+#    #+#             */
/*   Updated: 2017/09/01 16:48:41 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Simple_cloud.hpp"

Simple_cloud::Simple_cloud(void) : _cl_context(nullptr), _nb_particle(0)
{
}

Simple_cloud::Simple_cloud(size_t nb_particle, cl::Context const *cl_context)
{
	this->_nb_particle = nb_particle;
	this->_cl_context = cl_context;
	if (nb_particle != 0)
	{
		try
		{
			this->_gl_vbo = oGL_module::oGL_create_vbo(nb_particle *
				sizeof(GLfloat) * 3);
			oCL_module::oCL_create_cl_vbo(this->_gl_vbo, *cl_context, this->_cl_vbo);
		}
		catch (std::exception &e)
		{
			oGL_module::oGL_delete_vbo(this->_gl_vbo);
			throw Simple_cloud::Simple_cloudFailException();
		}
	}
}

Simple_cloud::~Simple_cloud(void)
{
	if (this->_nb_particle != 0)
		oGL_module::oGL_delete_vbo(this->_gl_vbo);
}

Simple_cloud::Simple_cloud(Simple_cloud const &src)
{
	*this = src;
}

Simple_cloud		&Simple_cloud::operator=(Simple_cloud const &rhs)
{
	if (this->_nb_particle != 0)
		oGL_module::oGL_delete_vbo(this->_gl_vbo);
	this->_nb_particle = rhs.get_nb_particle();
	this->_cl_context = rhs.get_cl_context();
	try
	{
		this->_gl_vbo = oGL_module::oGL_create_vbo(this->_nb_particle *
			sizeof(GLfloat) * 3);
		oCL_module::oCL_create_cl_vbo(this->_gl_vbo, *(rhs.get_cl_context()),
			this->_cl_vbo);
	}
	catch (std::exception &e)
	{
		oGL_module::oGL_delete_vbo(this->_gl_vbo);
		throw Simple_cloud::Simple_cloudFailException();
	}
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
