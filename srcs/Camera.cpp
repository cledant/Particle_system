/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 16:34:42 by cledant           #+#    #+#             */
/*   Updated: 2017/09/06 18:41:23 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Camera.hpp"

Camera::Camera(Input const &input, glm::vec3 const &pos, glm::vec3 const &world_up,
	glm::vec3 const &front, GLfloat yaw, GLfloat pitch) : _input(input),
	_world_up(world_up), _pos(pos), _front(front), _mouse_sensitivity(10.0f),
	_update_cam(true), _yaw(yaw), _pitch(pitch)
{
	this->update(0.0f);
}

Camera::~Camera(void)
{
}

Camera		&Camera::operator=(Camera const &rhs)
{
	static_cast<void>(rhs);
	return (*this);
}

void				Camera::update(float delta_time)
{
	if (this->_update_cam == true)
	{
		this->update_from_keyboard_input(delta_time);
		this->update_from_mouse_input();
		this->update_vector_matrix();
	}
}

void				Camera::toggle_update(void)
{
	this->_update_cam = (true) ? false : true;
}

glm::mat4 const		&Camera::getViewMatrix(void) const
{
	return (this->_view);
}

void				Camera::update_from_keyboard_input(float delta_time)
{
	float		velocity;

	velocity = delta_time * this->_movement_speed;
	if (this->_input.p_key[GLFW_KEY_UP] == PRESSED)
		this->_pos += velocity * this->_front;
	if (this->_input.p_key[GLFW_KEY_DOWN] == PRESSED)
		this->_pos -= velocity * this->_front;
	if (this->_input.p_key[GLFW_KEY_RIGHT] == PRESSED)
		this->_pos += velocity * this->_right;
	if (this->_input.p_key[GLFW_KEY_LEFT] == PRESSED)
		this->_pos -= velocity * this->_right;
}

void				Camera::update_from_mouse_input(void)
{
	this->_yaw += (this->_input.pos_x - this->_input.last_pos_x) *
		this->_mouse_sensitivity;
	this->_pitch += (this->_input.pos_y - this->_input.last_pos_y) *
		this->_mouse_sensitivity;
	if (this->_pitch > 89.0f)
		this->_pitch = 89.0f;
	if (this->_pitch > -89.0f)
		this->_pitch = -89.0f;
}

void				Camera::update_vector_matrix(void)
{
	this->_front.x = cos(glm::radians(this->_yaw)) * cos(glm::radians(this->_pitch));
	this->_front.x = sin(glm::radians(this->_pitch));
	this->_front.x = sin(glm::radians(this->_yaw)) * cos(glm::radians(this->_pitch));
	glm::normalize(this->_front);
	this->_right = glm::normalize(glm::cross(this->_front, this->_world_up));
	this->_up = glm::normalize(glm::cross(this->_right, this->_front));
	this->_view = glm::lookAt(this->_pos, this->_pos + this->_front, this->_up);
}

Camera::CameraFailException::CameraFailException(void)
{
	this->_msg = "Camera : Something failed";
}

Camera::CameraFailException::~CameraFailException(void) throw()
{
}
