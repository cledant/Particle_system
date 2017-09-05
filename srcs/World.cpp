/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   World.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 16:34:42 by cledant           #+#    #+#             */
/*   Updated: 2017/09/05 16:35:39 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "World.hpp"

World::World(Input const &input, Window const &win, glm::vec3 cam_pos, float time) :
   	_input(input), _window(window), _camera(input, cam_pos,
	glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), 0.0f, 0.0f)
{
	GLfloat ratio = static_cast<GLFloat>(win.cur_win_w) /
		static_cast<GLFloat>(win.cur_win_h);
	this->_perspective = glm::perspective(glm::radians(45.0f), ratio, 0.1f, 100.0f);
	this->_last_time = time;
	this->_delta_time = 0.0f;
	this->_camera.update(0.0f);
}

World::~World(void)
{
}

World::World(World const &src)
{
	static_cast<void>(src);
}

World		&World::operator=(World const &rhs)
{
	static_cast<void>(rhs);
	return (*this);
}

void		World::update(float time)
{
	std::vector<IEntity *>::iterator	it;

	this->delta_time = time - this->_last_time;
	this->_camera.update(this->_delta_time);
	for (it = this->_entity_list.begin(); it != this->_entity_list.end(),
			++it)
		it->update(time);
}

void		World::render(void)
{
	std::vector<IEntity *>::iterator	it;

	for (it = this->_entity_list.begin(); it != this->_entity_list.end(),
			++it)
		it->draw();
}

void		World::add_Simple_box(Shader const *shader, glm::vec3 pos,
				glm::vec3 scale)
{
	this->_entity_list.push_back(Simple_box(shader, &(this->_perspective),
		&(this->_camera, pos, scale)));
}

void		World::updatePerspective(float fov)
{
	GLfloat ratio = static_cast<GLFloat>(win.cur_win_w) /
		static_cast<GLFloat>(win.cur_win_h);
	this->_perspective = glm::perspective(glm::radians(fov), ratio, 0.1f, 100.0f);
}

World::WorldFailException::WorldFailException(void)
{
	this->_msg = "World : Something failed";
}

World::WorldFailException::~WorldFailException(void) throw()
{
}
