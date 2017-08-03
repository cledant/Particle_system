/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Glfw_manager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/02 13:21:30 by cledant           #+#    #+#             */
/*   Updated: 2017/08/03 19:29:18 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLFW_MANAGER_HPP
# define GLFW_MANAGER_HPP

# include "glfw3.h"
# include "Input.hpp"
# include "Window.hpp"
# include "GeneralException.hpp"
# include <functional>
# include <iostream>

class Glfw_manager
{
	public :
		Glfw_manager(void);
		virtual ~Glfw_manager(void);

		Input const		&getInput(void) const;
		Window const	&getWindow(void) const;

		void	run_glfw(void);
		void	create_ResizableWindow(std::string const name, int const major,
					int const minor, int const w, int const h);
		void	init_input_callback(void);

	private :
		Glfw_manager(Glfw_manager const &src);
		Glfw_manager		&operator=(Glfw_manager const &rhs);

		void	close_callback(GLFWwindow *win);
		void	keyboard_callback(GLFWwindow *win, int key, int scancode,
					int action, int mods);
		void	window_size_callback(GLFWwindow *win, int w, int h);
		void	cursor_position_callback(GLFWwindow *win, double xpos,
					double ypos);
		void	mouse_button_callback(GLFWwindow *win, int button,
					int action, int mods);
		void	error_callback(int error, const char *what);

		Input	_input;
		Window	_window;

	class InitFailException : public GeneralException
	{
		public :

			explicit InitFailException(void);
			virtual ~InitFailException(void) throw();
	};

	class WindowFailException : public GeneralException
	{
		public :

			explicit WindowFailException(void);
			virtual ~WindowFailException(void) throw();
	};
};

#endif
