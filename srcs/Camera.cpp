/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 16:34:42 by cledant           #+#    #+#             */
/*   Updated: 2017/09/04 19:40:20 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Camera.hpp"

Camera::Camera(Input const &input, glm::vec3 const &pos, glm::vec3 const &world_up,
	glm::vec3 const &front) : _input(input), _world_up(world_up), _pos(pos),
	_front(front)
{
	this->update();
}

Camera::~Camera(void)
{
}

Camera::Camera(Camera const &src)
{
	static_cast<void>(src);
}

Camera		&Camera::operator=(Camera const &rhs)
{
	static_cast<void>(rhs);
	return (*this);
}

void		Camera::update(float delta_time)
{
	
}

Camera::CameraFailException::CameraFailException(void)
{
	this->_msg = "Camera : Something failed";
}

Camera::CameraFailException::~CameraFailException(void) throw()
{
}
