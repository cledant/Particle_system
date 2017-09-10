/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   World.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 16:34:42 by cledant           #+#    #+#             */
/*   Updated: 2017/09/10 11:34:35 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "World.hpp"

World::World(Input const &input, Window const &win, glm::vec3 cam_pos) :
   	_input(input), _window(win), _camera(input, cam_pos,
	glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), -90.0f, 0.0f)
{
	GLfloat ratio = static_cast<GLfloat>(win.cur_win_w) /
		static_cast<GLfloat>(win.cur_win_h);
	this->_fov = 45.0f;
	this->_perspective = glm::perspective(glm::radians(this->_fov), ratio, 0.1f,
		100.0f);
	this->_camera.update(0.0f, true);
}

World::~World(void)
{
	static_cast<void>(this->_input);
	std::vector<IEntity *>::iterator	it;

	for (it = this->_entity_list.begin(); it != this->_entity_list.end(); ++it)
		delete *it;
}

World		&World::operator=(World const &rhs)
{
	static_cast<void>(rhs);
	return (*this);
}

void		World::update(float delta_time, bool mouse_exclusive_to_manager)
{
	std::vector<IEntity *>::iterator	it;

	this->_camera.update(delta_time, mouse_exclusive_to_manager);
	if (this->_window.resized == true)
		this->updatePerspective(this->_fov);
	for (it = this->_entity_list.begin(); it != this->_entity_list.end(); ++it)
		(*it)->update(delta_time);
}

void		World::render(void)
{
	std::vector<IEntity *>::iterator	it;

	oGL_module::oGL_clear_buffer(0.2f, 0.3f, 0.3f);
	for (it = this->_entity_list.begin(); it != this->_entity_list.end(); ++it)
		(*it)->draw();
}

void		World::add_Simple_box(Shader const *shader, glm::vec3 const &pos,
				glm::vec3 const &scale)
{
	this->_entity_list.push_back(new Simple_box(shader, &(this->_perspective),
		&(this->_camera), pos, scale));
}

void		World::add_Cubemap(Shader const *shader, Texture const *texture)
{
	this->_entity_list.push_back(new Cubemap(shader, &(this->_perspective),
		&(this->_camera), texture));
}

void		World::updatePerspective(float fov)
{
	GLfloat ratio = static_cast<GLfloat>(this->_window.cur_win_w) /
		static_cast<GLfloat>(this->_window.cur_win_h);
	this->_perspective = glm::perspective(glm::radians(fov), ratio, 0.1f, 100.0f);
}

World::WorldFailException::WorldFailException(void)
{
	this->_msg = "World : Something failed";
}

World::WorldFailException::~WorldFailException(void) throw()
{
}
