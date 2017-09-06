/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shader.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 14:06:22 by cledant           #+#    #+#             */
/*   Updated: 2017/09/06 11:54:33 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Shader.hpp"

Shader::Shader(std::string const &name, std::string const &vs_path,
		std::string const &fs_path) : _name(name)
{
	GLuint		vs = 0;
	Gluint		fs = 0;

	try
	{
		vs = Shader::load_shader(vs_path, GL_VERTEX_SHADER);
		fs = Shader::load_shader(fs_path, GL_FRAGMENT_SHADER);
		this->_shader_program = Shader::compile_shader(vs, fs);
	}
	catch (std::exception &e)
	{
		if (vs != 0)
			glDeleteShader(vs);
		if (fs != 0)
			glDeleteShader(fs);
		throw e;
	}
	glDeleteShader(vs);
	glDeleteShader(fs);
}

Shader::~Shader(void)
{
	glDeleteShader(this->_shader_program);
}

Shader::Shader(Shader const &src)
{
	static_cast<void>(src);
}

Shader		&Shader::operator=(Shader const &rhs)
{
	static_cast<void>(rhs);
	return (*this);
}

std::string const		&Shader::getName(void) const
{
	return (this->_name);
}

GLuint					Shader::getShaderProgram(void) const
{
	return (this->_shader_program);
}

void					Shader::use(void)
{
	glUseProgram(this->_shader_program);
}

void			Shader::setMat4(GLint uniform_id, glm::mat const &mat4)
{
	glUniformMatrix4fv(uniform_id, 1, GL_FALSE, &mat4);
}

GLuint			Shader::load_shader(std::string const &path, GLenum type)
{
	std::string		content;
	GLuint			shader = 0;
	GLuint			success;

	std::cout << "Loading : " << path << std::endl;
	Shader::read_file(path, content);
	if ((shader = glCreateShader(type)) == 0)
		throw Shader::AllocationException();
	glShaderSource(shader, 1, &(content.c_str()), NULL);
	glCompileShader(shader);
	glGetShaderivi(shader, GL_COMPILE_STATUS, &success);
	if (success != GL_TRUE)
	{
		Shader::get_compile_error(shader);
		glDeleteShader(shader);
		throw Shader::CompileException();
	}
	return (shader);
}

void			Shader::compile_program(GLuint vs, GLuint fs)
{
	GLuint		prog = 0;
	GLuint		success;

	if ((prog = glCreateProgram()) == 0)
		throw Shader::AllocationException();
	glAttachShader(prog, vs);
	glAttachShader(prog, fs);
	glLinkProgram(prog);
	glGetProgramiv(prog, GL_LINK_STATUS, &success);
	if (success != GL_TRUE)
		throw Shader::LinkException();
	return (prog);
}

void			Shader::get_shader_error(GLuint shader)
{
	char	msg[4096];
	int		msg_len;

	glGetShaderInfoLog(shader, 4095, &msg_len, msg);
	msg[4095] = '\0';
	std::cout << msg << std::endl;
}

void			Shader::read_file(std::string const &path, std::string &content)
{
	std::fstream	fs;

	try
	{
		fs.open(path, std::fstream::in);
		content.assign((std::istreambuf_iterator<char>(fs)),
			std::istreambuf_iterator<char>());
		fs.close();
	}
	catch (std::exception &e)
	{
		throw Shader::FailToOpenException(path);
	}
}

Shader::FailOpenFileException::FailOpenFileException(void)
{
	this->_msg = "Shader : Failed to find to open file";
}

Shader::FailOpenFileException::~FailOpenFileException(void) throw()
{
}

Shader::AllocationException::AllocationException(void)
{
	this->_msg = "Shader : Failed to allocate memory";
}

Shader::AllocationException::~AllocationException(void) throw()
{
}

Shader::CompileException::CompileException(void)
{
	this->_msg = "Shader : Failed to compile shader";
}

Shader::CompileException::~CompileException(void) throw()
{
}

Shader::LinkException::LinkException(void)
{
	this->_msg = "Shader : Failed to link shader to program";
}

Shader::LinkException::~LinkException(void) throw()
{
}
