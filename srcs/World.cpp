/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   World.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 16:34:42 by cledant           #+#    #+#             */
/*   Updated: 2017/09/11 12:54:43 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "World.hpp"

World::World(Input const &input, Window const &win, glm::vec3 cam_pos,
		float max_fps, size_t max_frame_skip) : _input(input), _window(win),
		_camera(input, cam_pos, glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -1.0f), -90.0f, 0.0f), _fov(45.0f), _max_fps(max_fps),
		_max_frame_skip(max_frame_skip), _next_update_tick(0.0f),
		_last_update_tick(0.0f), _delta_tick(0.0f), _skip_loop(0)
{
	if (max_frame_skip == 0)
		throw World::WorldFailException();
	GLfloat ratio = static_cast<GLfloat>(win.cur_win_w) /
		static_cast<GLfloat>(win.cur_win_h);
	this->_tick = 1.0f / this->_max_fps;
	this->_perspective = glm::perspective(glm::radians(this->_fov), ratio, 0.1f,
		100.0f);
	this->_camera.update(true);
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

void		World::update(bool mouse_exclusive_to_manager)
{
	std::vector<IEntity *>::iterator	it;

	this->_camera.update(mouse_exclusive_to_manager);
	if (this->_window.resized == true)
		this->updatePerspective(this->_fov);
	this->_perspec_mult_view = this->_perspective * this->_camera.getViewMatrix();
	for (it = this->_entity_list.begin(); it != this->_entity_list.end(); ++it)
		(*it)->update(this->_delta_tick);
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
	this->_entity_list.push_back(new Simple_box(shader, &(this->_perspec_mult_view),
		pos, scale));
}

void		World::add_Cubemap(Shader const *shader, Texture const *texture,
				glm::vec3 const &pos, glm::vec3 const &scale)
{
	this->_entity_list.push_back(new Cubemap(shader, &(this->_perspec_mult_view),
		texture, pos, scale));
}

void		World::updatePerspective(float fov)
{
	GLfloat ratio = static_cast<GLfloat>(this->_window.cur_win_w) /
		static_cast<GLfloat>(this->_window.cur_win_h);
	this->_perspective = glm::perspective(glm::radians(fov), ratio, 0.1f, 100.0f);
}

void		World::reset_update_timer(float time)
{
	this->_next_update_tick = time;
	this->_last_update_tick = time;
}

void		World::reset_skip_loop(void)
{
	this->_skip_loop = 0;
}

bool		World::should_be_updated(float time)
{
	if (time > this->_next_update_tick &&
			this->_skip_loop < this->_max_frame_skip)
	{
		(this->_skip_loop)++;
		this->_next_update_tick += this->_tick;
		this->_delta_tick = time - this->_last_update_tick;
		this->_last_update_tick = time;
		return (true);
	}
	return (false);
}

World::WorldFailException::WorldFailException(void)
{
	this->_msg = "World : Something failed";
}

World::WorldFailException::~WorldFailException(void) throw()
{
}
