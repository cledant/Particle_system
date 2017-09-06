/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oGL_module.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/30 13:58:09 by cledant           #+#    #+#             */
/*   Updated: 2017/09/06 15:56:18 by cledant          ###   ########.fr       */
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

void			oGL_module::oGL_check_error(void)
{
	if (glGetError() != GL_NO_ERROR)
		throw oGL_module::oGLFailException();
}

GLuint			oGL_module::oGL_create_vbo(size_t size, void *data)
{
	GLuint		new_vbo;

	glGenBuffers(1, &new_vbo);
	oGL_module::oGL_check_error();
	glBindBuffer(GL_ARRAY_BUFFER, new_vbo);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), data,
		GL_STATIC_DRAW);
	oGL_module::oGL_check_error();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return (new_vbo);
}

void			oGL_module::oGL_delete_vbo(GLuint vbo)
{
	glDeleteBuffers(1, &vbo);
}

GLuint			oGL_module::oGL_create_vao(void)
{
	GLuint		new_vao;

	glGenVertexArrays(1, &new_vao);
	oGL_module::oGL_check_error();
	return (new_vao);
}

void			oGL_module::oGL_set_vao_parameters(GLuint vbo, GLuint vao,
					GLuint index, GLint size, GLsizei stride,
					size_t shift)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindVertexArray(vao);
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride,
		reinterpret_cast<void *>(shift));
	glEnableVertexAttribArray(index);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void			oGL_module::oGL_delete_vao(GLuint vao)
{
	glDeleteVertexArrays(1, &vao);
}

void			oGL_module::oGL_clear_buffer(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void			oGL_module::oGL_update_framebuffer(int width, int heigth)
{
	glViewport(0, 0, width, heigth);
}

void			oGL_module::oGL_enable_depth(void)
{
	glEnable(GL_DEPTH_TEST);
}

bool			oGL_module::oGL_getUniformID(std::string const &name,
					GLuint prog, GLint *uniform_id)
{
	if (uniform_id == nullptr)
		return (false);
	*uniform_id = glGetUniformLocation(prog, name.c_str());
	if (glGetError() != GL_NO_ERROR)
		return (false);
	return (true);
}

void			oGL_module::oGL_draw_filled(GLuint vbo, GLuint vao, size_t
					nb_faces)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindVertexArray(vao);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, nb_faces);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void			oGL_module::add_shader(std::string const &name,
					std::string const &vs_path, std::string const &fs_path)
{
	this->_shader_list.push_back({name, vs_path, fs_path});
}

Shader const	&oGL_module::getShader(std::string const &name)
{
	std::vector<Shader>::iterator		it;

	for (it = this->_shader_list.begin(); it != this->_shader_list.end(); ++it)
	{
		if (it->getName().compare(name) == 0)
			return (*it);
	}
	throw oGL_module::ShaderNotFoundException(name);
}

void			oGL_module::delete_all_shaders(void)
{
	std::vector<Shader>::iterator		it;

	for (it = this->_shader_list.begin(); it != this->_shader_list.end(); ++it)
		glDeleteShader(it->getShaderProgram());
}

oGL_module::ShaderNotFoundException::ShaderNotFoundException(void)
{
	this->_msg = "OpenGL : Failed to find requested shader";
}

oGL_module::ShaderNotFoundException::ShaderNotFoundException(std::string const &name)
{
	this->_msg = "OpenGL : Failed to find shader : ";
	this->_msg += name.c_str();
}

oGL_module::ShaderNotFoundException::~ShaderNotFoundException(void) throw()
{
}

oGL_module::oGLFailException::oGLFailException(void)
{
	this->_msg = "OpenGL : Something failed !";
}

oGL_module::oGLFailException::~oGLFailException(void) throw()
{
}
