/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Simple_box.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/05 17:30:41 by cledant           #+#    #+#             */
/*   Updated: 2017/09/08 15:55:50 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Simple_box.hpp"

Simple_box::Simple_box(Shader const *shader, glm::mat4 const *perspective,
	Camera const *camera, glm::vec3 pos, glm::vec3 scale) : _shader(shader),
	_perspective(perspective), _cam(camera), _vbo(0), _vao(0)
{
	try
	{
		this->_vbo = oGL_module::oGL_create_vbo(sizeof(float) * 6 * 6 * 6,
			static_cast<void *>(Simple_box::_vertices));
		this->_vao = oGL_module::oGL_create_vao();
		oGL_module::oGL_set_vao_parameters(this->_vao, this->_vbo, 0, 3,
			sizeof(GLfloat) * 6, 0);
		oGL_module::oGL_set_vao_parameters(this->_vao, this->_vbo, 1, 3,
			sizeof(GLfloat) * 6, sizeof(GLfloat) * 3);
	}
	catch (std::exception &e)
	{
		oGL_module::oGL_delete_vao(this->_vao);
		oGL_module::oGL_delete_vbo(this->_vbo);
		throw Simple_box::InitException();
	}
	this->_model = glm::mat4(1.0f);
	this->_pos = pos;
	this->_scale = scale;
	this->update(0.0f);
}

Simple_box::~Simple_box(void)
{
	oGL_module::oGL_delete_vao(this->_vao);
	oGL_module::oGL_delete_vbo(this->_vbo);
}

Simple_box::Simple_box(Simple_box const &src)
{
	static_cast<void>(src);
}

Simple_box			&Simple_box::operator=(Simple_box const &rhs)
{
	static_cast<void>(rhs);
	return (*this);
}

void				Simple_box::update(float time)
{
	static_cast<void>(time);
	if (this->_shader == nullptr || this->_perspective == nullptr ||
			this->_cam == nullptr)
	{
		std::cout << "Warning : Can't update Simple_box" << std::endl;
		return ;
	}
//	Simple_box::print_matrix(this->_model, "base model");
//	Simple_box::print_vec3(this->_pos, "Vector pos");
//	Simple_box::print_matrix(*(this->_perspective), "perspec");
//	Simple_box::print_matrix(this->_cam->getViewMatrix(), "view");
	this->_model = glm::scale(glm::translate(this->_model, this->_pos),
		this->_scale);
//	this->_model = glm::translate(this->_model, this->_pos);
//	Simple_box::print_matrix(this->_model, "model");
	this->_total = *(this->_perspective) * this->_cam->getViewMatrix() *
		this->_model;
//	Simple_box::print_matrix(this->_total, "total");
}

void				Simple_box::draw(void)
{
	GLint	uniform_id;

	if (this->_shader == nullptr || this->_perspective == nullptr ||
			this->_cam == nullptr || oGL_module::oGL_getUniformID("mat_total",
			this->_shader->getShaderProgram(), &uniform_id) == false)
	{
		std::cout << "Warning : Can't draw Simple_box" << std::endl;
		return ;
	}
//	Simple_box::print_matrix(this->_total, "total");
	this->_shader->use();
	this->_shader->setMat4(uniform_id, this->_total);
	oGL_module::oGL_draw_filled(this->_vbo, this->_vao, Simple_box::_nb_faces);
}

void				Simple_box::setPosition(glm::vec3 const &pos)
{
	this->_pos = pos;
}

void				Simple_box::setScale(glm::vec3 const &scale)
{
	this->_scale = scale;
}

void				Simple_box::print_matrix(glm::mat4 const &mat,
						std::string const &name)
{
	std::cout << "=======" << std::endl;
	std::cout << name << std::endl;
	std::cout << (mat)[0][0] << " " << (mat)[0][1] << " " << (mat)[0][2] << " " << (mat)[0][3] << std::endl;
	std::cout << (mat)[1][0] << " " << (mat)[1][1] << " " << (mat)[1][2] << " " << (mat)[1][3] << std::endl;
	std::cout << (mat)[2][0] << " " << (mat)[2][1] << " " << (mat)[2][2] << " " << (mat)[2][3] << std::endl;
	std::cout << (mat)[3][0] << " " << (mat)[3][1] << " " << (mat)[3][2] << " " << (mat)[3][3] << std::endl;
	std::cout << "=======" << std::endl;
}

void				Simple_box::print_vec3(glm::vec3 const &vec,
						std::string const &name)
{
	std::cout << "=======" << std::endl;
	std::cout << name << std::endl;
	std::cout << vec.x << " " << vec.y << " " << vec.z << std::endl;
	std::cout << "=======" << std::endl;
}

glm::mat4 const		&Simple_box::getTotalMatrix(void)
{
	return (this->_total);
}

Simple_box::InitException::InitException(void)
{
	this->_msg = "Simple_Box : Object initialization failed";
}

Simple_box::InitException::~InitException(void) throw()
{
}

float			Simple_box::_vertices[] =
{
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f
};

size_t			Simple_box::_nb_faces = 36;
