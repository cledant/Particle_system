/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   World.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 16:34:42 by cledant           #+#    #+#             */
/*   Updated: 2017/09/23 17:42:40 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "World.hpp"

World::World(Input const &input, Window const &win, glm::vec3 cam_pos,
		float max_fps, size_t max_frame_skip) :
		_active(nullptr), _input(input), _window(win),
		_camera(input, cam_pos, glm::vec3(0.0f, 1.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, -1.0f), -90.0f, 0.0f),
		_fov(45.0f), _max_fps(max_fps),
		_max_frame_skip(max_frame_skip), _next_update_tick(0.0f),
		_last_update_tick(0.0f), _delta_tick(0.0f), _skip_loop(0),
		_input_timer(0.0f), _input_mouse_timer(0.0f)
{
	if (max_frame_skip == 0)
		throw World::WorldFailException();
	GLfloat ratio = static_cast<GLfloat>(win.cur_win_w) /
		static_cast<GLfloat>(win.cur_win_h);
	this->_tick = 1.0f / this->_max_fps;
	this->_perspective = glm::perspective(glm::radians(this->_fov), ratio, 0.1f,
		400.0f);
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

void		World::update(void)
{
	std::vector<IEntity *>::iterator	it;

	this->_camera.update(this->_input.mouse_exclusive);
	if (this->_window.resized == true)
		this->updatePerspective(this->_fov);
	this->_perspec_mult_view = this->_perspective * this->_camera.getViewMatrix();
	if (this->_active != nullptr)
	{
		if (this->_active->update_mouse_interaction(this->_input, this->_window,
				this->_camera.getPos(), std::vector<glm::vec3 const *>{
				&(this->_camera.getFront()), &(this->_camera.getUp()),
				&(this->_camera.getRight())}, this->_input_mouse_timer) == true)
			this->_input_mouse_timer = 0.0f;
		else if (this->_input_mouse_timer < 1.0f)
			this->_input_mouse_timer += this->_tick;
		if (this->_active->update_keyboard_interaction(this->_input,
				this->_input_timer) == true)
			this->_input_timer = 0.0f;
		else if (this->_input_timer < 1.0f)
			this->_input_timer += this->_tick;
	}
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

IEntity		*World::add_Simple_box(Shader const *shader, glm::vec3 const &pos,
				glm::vec3 const &scale)
{
	IEntity		*ptr;

	ptr = new Simple_box(shader, &(this->_perspec_mult_view), pos, scale);
	this->_entity_list.push_back(ptr);
	return (ptr);
}

IEntity		*World::add_Cubemap(Shader const *shader, Texture const *texture,
				glm::vec3 const &pos, glm::vec3 const &scale)
{
	IEntity		*ptr;

	ptr = new Cubemap(shader, &(this->_perspec_mult_view), texture, pos, scale);
	this->_entity_list.push_back(ptr);
	return (ptr);
}

IEntity		*World::add_Simple_cloud(Simple_cloud::Params &params)
{
	IEntity		*ptr;
	
	params.refresh_tick = this->_tick;
	params.perspec_mult_view = &(this->_perspec_mult_view);
	ptr = new Simple_cloud(params);
	this->_entity_list.push_back(ptr);
	return (ptr);
}

void		World::setActiveInteractive(IInteractive *ptr)
{
	this->_active = ptr;
}

void		World::updatePerspective(float fov)
{
	GLfloat ratio = static_cast<GLfloat>(this->_window.cur_win_w) /
		static_cast<GLfloat>(this->_window.cur_win_h);
	this->_perspective = glm::perspective(glm::radians(fov), ratio, 0.1f, 400.0f);
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
