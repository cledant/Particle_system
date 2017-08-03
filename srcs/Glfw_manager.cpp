/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Glfw_manager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/03 11:30:26 by cledant           #+#    #+#             */
/*   Updated: 2017/08/03 20:55:07 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Glfw_manager.hpp"

Glfw_manager::Glfw_manager(void) : _input(), _window()
{
	auto	error_callback = [](int error, char const *what)
	{
		std::cout << "GLFW error code : " << error << std::endl;
		std::cout << what << std::endl;
	};

	glfwSetErrorCallback(error_callback);
}

Glfw_manager::~Glfw_manager(void)
{
	if (!(this->_window.win))
	{
		glfwDestroyWindow(this->_window.win);
		_nb_active_win--;
	}
	if (_nb_active_win == 0)
		glfwTerminate();
}

Glfw_manager::Glfw_manager(Glfw_manager const &src) : _input(), _window()
{
	static_cast<void>(src);
}

Glfw_manager		&Glfw_manager::operator=(Glfw_manager const &rhs)
{
	static_cast<void>(rhs);
	return (*this);
}

Input const			&Glfw_manager::getInput(void) const
{
	return (this->_input);
}

Window const		&Glfw_manager::getWindow(void) const
{
	return (this->_window);
}

void				Glfw_manager::create_ResizableWindow(std::string const name,
						int const major, int const minor, int const w,
						int const h)
{
	auto	close_callback = [](GLFWwindow *win)
	{
		glfwSetWindowShouldClose(win, GLFW_TRUE);
	};

	auto	window_size_callback = [](GLFWwindow *win, int w, int h)
	{
		static_cast<void>(win);
		static_cast<Glfw_manager *>(glfwGetWindowUserPointer(win))->_window.cur_win_h = h;
		static_cast<Glfw_manager *>(glfwGetWindowUserPointer(win))->_window.cur_win_w = w;
	};

	if (glfwInit() != GLFW_TRUE)
		throw Glfw_manager::InitFailException();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	if ((this->_window.win = glfwCreateWindow(w, h, name.c_str(), NULL,
			NULL)) == NULL)
		throw Glfw_manager::WindowFailException();
	this->_window.cur_win_h = h;
	this->_window.cur_win_w = w;
	_nb_active_win++;
	glfwSetWindowCloseCallback(this->_window.win, close_callback);
	glfwSetWindowSizeCallback(this->_window.win, window_size_callback);
	glfwSetInputMode(this->_window.win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetWindowSizeLimits(this->_window.win, this->_window.min_win_w,
		this->_window.min_win_h, this->_window.max_win_w, this->_window.max_win_h);
	glfwSetWindowUserPointer(this->_window.win, this);
	glfwMakeContextCurrent(this->_window.win);
}

void				Glfw_manager::init_input_callback(void)
{
	auto	keyboard_callback = [](GLFWwindow *win, int key, int scancode,
				int action, int mods)
	{
		static_cast<void>(scancode);
		static_cast<void>(mods);
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(win, GL_TRUE);
		if (key >= 0 && key < 1024)
		{
			if (action == GLFW_PRESS)
				static_cast<Glfw_manager *>(glfwGetWindowUserPointer(win))->_input.p_key[key] = PRESSED;
			else if (action == GLFW_RELEASE)
				static_cast<Glfw_manager *>(glfwGetWindowUserPointer(win))->_input.p_key[key] = RELEASED;
		}
	};

	auto	cursor_position_callback = [](GLFWwindow *win, double xpos, double ypos)
	{
		static_cast<void>(win);
		static_cast<Glfw_manager *>(glfwGetWindowUserPointer(win))->_input.pos_x = static_cast<GLfloat>(xpos);
		static_cast<Glfw_manager *>(glfwGetWindowUserPointer(win))->_input.pos_y = static_cast<GLfloat>(ypos);
	};

	auto	mouse_button_callback = [](GLFWwindow *win, int button, int action,
				int mods)
	{
		static_cast<void>(win);
		static_cast<void>(mods);
		if (button >= 0 && button < 9)
		{
			if (action == GLFW_PRESS)
				static_cast<Glfw_manager *>(glfwGetWindowUserPointer(win))->_input.p_mouse[button] = PRESSED;
			else if (action == GLFW_RELEASE)
				static_cast<Glfw_manager *>(glfwGetWindowUserPointer(win))->_input.p_mouse[button] = RELEASED;
		}
	};

	glfwSetKeyCallback(this->_window.win, keyboard_callback);
	glfwSetMouseButtonCallback(this->_window.win, mouse_button_callback);
	glfwSetCursorPosCallback(this->_window.win, cursor_position_callback);
}

Glfw_manager::InitFailException::InitFailException(void)
{
	this->_msg = "GLFW : Initilization failed !";
}


Glfw_manager::InitFailException::~InitFailException(void) throw()
{
}

Glfw_manager::WindowFailException::WindowFailException(void)
{
	this->_msg = "GLFW : Window creation failed !";
}


Glfw_manager::WindowFailException::~WindowFailException(void) throw()
{
}

size_t		Glfw_manager::_nb_active_win = 0;
