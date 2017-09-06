/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Simple_box.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/05 17:30:41 by cledant           #+#    #+#             */
/*   Updated: 2017/09/06 15:35:24 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Simple_box.hpp"

Simple_box::Simple_box(Shader const *shader, glm::mat4 const *perspective,
	Camera const *camera, glm::vec3 pos, glm::vec3 scale) : _shader(shader),
	_perspective(perspective), _cam(camera), _vbo(0), _vao(0)
{
	try
	{
		this->_vbo = oGL_module::oGL_create_vbo(sizeof(this->_verticies),
			static_cast<void *>(this->_verticies));
		this->_vao = oGL_module::oGL_create_vao();
		oGL_module::oGL_set_vao_parameters(0, 3, sizeof(GLfloat) * 3,
			0);
		oGL_module::oGL_set_vao_parameters(1, 3, sizeof(GLfloat) * 3,
			sizeof(GLfloat) * 3);
	}
	catch (std::exception &e)
	{
		oGL_module::oGL_delete_vao(this->_vao);
		oGL_module::oGL_delete_vbo(this->_vbo);
		throw Simple_box::Simple_boxInitFail();
	}
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
	this->_model = glm::translate(this->_model, this->_pos) * this->_scale;
	this->_total = this->_perspective * this->_camera.getViewMatrix() *
		this->_model;
}

void				Simple_box::draw(void)
{
	GLint	uniform_id

	if (this->_shader == nullptr || this->_perspective == nullptr ||
			this->_cam == nullptr || oGL_module::oGL_getUniformID("mat_total",
			&uniform_id) == false)
	{
		std::cout << "Warning : Can't draw Simple_box" << std::endl;
		return ;
	}
	this->_shader.use();
	this->_shader.setMat4(uniform_id, this->_total);
	oGL_module::oGL_draw_filled(this->_vbo, this->_vao);
}

void				Simple_box::setPosition(glm::vec3 const &pos)
{
	this->_pos = pos;
}

void				Simple_box::setScale(glm::vec3 const &scale)
{
	this->_scale = scale;
}

glm::mat4 const		&Simple_box::getTotalMatrix(void)
{
	return (this->_total);
}

static float		_verticies[] =
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

static size_t		_nb_faces = 12;
