/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Texture.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/09 17:38:14 by cledant           #+#    #+#             */
/*   Updated: 2017/09/09 17:50:51 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Texture.hpp"

Texture::Texture(std::string const &name, std::vector<std::string> const &files,
	t_type_tex type) : _name(name), _tex_id(0)
{
	switch (type)
	{
		case TEX_CUBE :
			this->_tex_id = Texture::load_cubemap(files);
			break;
		default :
			throw TypeException();
			break;
	}
}

Texture::~Texture(void)
{
}

Texture::Texture(Texture const &src)
{
	this->_name = src.getName();
	this->_tex_id = src.getTextureID();
}

Texture		&Texture::operator=(Texture const &rhs)
{
	this->_name = rhs.getName();
	this->_tex_id = rhs.getTextureProgram();
	return (*this);
}

std::string const		&Texture::getName(void) const
{
	return (this->_name);
}

GLuint					Texture::getTextureID(void) const
{
	return (this->_tex_id);
}

GLuint			Texture::load_cubemap(std::vector<std::string> const &files)
{
	std::string		content;
	GLuint			shader = 0;
	GLint			success;
	char const		*content_array;

	std::cout << "Loading : " << path << std::endl;
	Texture::read_file(path, content);
	if ((shader = glCreateTexture(type)) == 0)
		throw Texture::AllocationException();
	content_array = content.c_str();
	glTextureSource(shader, 1, &content_array, NULL);
	glCompileTexture(shader);
	glGetTextureiv(shader, GL_COMPILE_STATUS, &success);
	if (success != GL_TRUE)
	{
		Texture::get_shader_error(shader);
		glDeleteTexture(shader);
		throw Texture::CompileException();
	}
	return (shader);
}

GLuint			Texture::compile_program(GLuint vs, GLuint fs)
{
	GLuint		prog = 0;
	GLint		success;

	if ((prog = glCreateProgram()) == 0)
		throw Texture::AllocationException();
	glAttachTexture(prog, vs);
	glAttachTexture(prog, fs);
	glLinkProgram(prog);
	glGetProgramiv(prog, GL_LINK_STATUS, &success);
	if (success != GL_TRUE)
	{
		Texture::get_shader_error(prog);
		throw Texture::LinkException();
	}
	return (prog);
}

void			Texture::get_shader_error(GLuint shader)
{
	char	msg[4096];
	int		msg_len;

	glGetTextureInfoLog(shader, 4095, &msg_len, msg);
	msg[4095] = '\0';
	std::cout << msg << std::endl;
}

void			Texture::read_file(std::string const &path, std::string &content)
{
	std::fstream	fs;

	try
	{
		fs.exceptions(std::fstream::failbit | std::fstream::badbit);
		fs.open(path, std::fstream::in);
		content.assign((std::istreambuf_iterator<char>(fs)),
			std::istreambuf_iterator<char>());
		fs.close();
	}
	catch (std::exception &e)
	{
		throw Texture::FileOpenException(path);
	}
}

Texture::FileOpenException::FileOpenException(std::string const &path)
{
	this->_msg = "Texture : Failed to find to open file : ";
	this->_msg += path.c_str();
}

Texture::FileOpenException::FileOpenException(void)
{
	this->_msg = "Texture : Failed to find to open file";
}

Texture::FileOpenException::~FileOpenException(void) throw()
{
}

Texture::AllocationException::AllocationException(void)
{
	this->_msg = "Texture : Failed to allocate memory";
}

Texture::AllocationException::~AllocationException(void) throw()
{
}
