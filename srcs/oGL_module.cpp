/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oGL_module.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/30 13:58:09 by cledant           #+#    #+#             */
/*   Updated: 2017/09/01 16:31:10 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "oGL_module.hpp"

oGL_module::oGL_module(void)
{
}

oGL_module::~oGL_module(void)
{
}

oGL_module::oGL_module(oGL_module const &src)
{
	static_cast<void>(src);
}

oGL_module		&oGL_module::operator=(oGL_module const &rhs)
{
	static_cast<void>(rhs);
	return (*this);
}

GLuint			oGL_module::oGL_create_vbo(size_t size)
{
	GLuint		new_vbo;

	glGenBuffers(1, &new_vbo);
	oGL_module::oGL_check_error();
	glBindBuffer(GL_ARRAY_BUFFER, new_vbo);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), 0, GL_DYNAMIC_DRAW);
	oGL_module::oGL_check_error();
	return (new_vbo);
}

void			oGL_module::oGL_delete_vbo(GLuint vbo)
{
	glDeleteBuffers(1, &vbo);
}

void			oGL_module::oGL_check_error(void)
{
	if (glGetError() != GL_NO_ERROR)
		throw oGL_module::oGLFailException();
}

void			oGL_module::read_file(std::string const &path, std::string &content)
{
	std::fstream	fs;

	fs.open(path, std::fstream::in);
	content.assign((std::istreambuf_iterator<char>(fs)),
		std::istreambuf_iterator<char>());
	fs.close();
}

oGL_module::oGLFailException::oGLFailException(void)
{
	this->_msg = "OpenGL : Something failed !";
}

oGL_module::oGLFailException::~oGLFailException(void) throw()
{
}
