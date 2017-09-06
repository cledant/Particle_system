/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oGL_module.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/30 13:58:09 by cledant           #+#    #+#             */
/*   Updated: 2017/09/06 12:58:19 by cledant          ###   ########.fr       */
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

GLuint			oGL_module::oGL_create_vbo(size_t size)
{
	GLuint		new_vbo;

	glGenBuffers(1, &new_vbo);
	oGL_module::oGL_check_error();
	glBindBuffer(GL_ARRAY_BUFFER, new_vbo);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), 0, GL_STATIC_DRAW);
	oGL_module::oGL_check_error();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return (new_vbo);
}

void			oGL_module::oGL_delete_vbo(GLuint vbo)
{
	glDeleteBuffers(1, &vbo);
}

GLuint			oGL_module::oGL_create_vao(GLuint vbo, size_t size)
{
	GLuint		new_vao;

	glGenVertexArrays(1, &new_vao);
	oGL_module::oGL_check_error();
	glBindBuffer(GL_ARRAY_BUFFER, new_vbo);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), 0, GL_STATIC_DRAW);
	oGL_module::oGL_check_error();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return (new_vbo);
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
	glBindBuffer(GL_BUFFER_ARRAY, vbo);
	glBindVertexArrays(vao);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, nb_faces);
	glBindVertexArray(0);
	glBindBuffer(0);
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
			return (it);
	}
	throw oGL_module::ShaderNotFoundException(name);
}

void			oGL_module::delete_all_shaders(void)
{
	std::vector<Shader>::iterator		it;

	for (it = this->_shader_list.begin(); it != this->_shader_list.end(); ++it)
		delete it;
}

void			oGL_module::read_file(std::string const &path, std::string &content)
{
	std::fstream	fs;

	fs.open(path, std::fstream::in);
	content.assign((std::istreambuf_iterator<char>(fs)),
		std::istreambuf_iterator<char>());
	fs.close();
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
