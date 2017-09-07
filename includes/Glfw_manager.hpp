/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Glfw_manager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/02 13:21:30 by cledant           #+#    #+#             */
/*   Updated: 2017/09/07 14:30:58 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLFW_MANAGER_HPP
# define GLFW_MANAGER_HPP

# define THIS_GLFW static_cast<Glfw_manager *>(glfwGetWindowUserPointer(win))

# include "glfw3.h"
# include "oGL_module.hpp"
# include "Input.hpp"
# include "Window.hpp"
# include "GeneralException.hpp"
# include <cfenv>
# include <iostream>
# include <sstream>
# include <iomanip>

class Glfw_manager
{
	public :

		Glfw_manager(void);
		virtual ~Glfw_manager(void);

		static void				run_manager(void);
		static void				close_manager(void);
		static size_t			getActiveWindowNumber(void);
		static float			getTime(void);

		Input const				&getInput(void) const;
		Window const			&getWindow(void) const;
		bool					getMouseMode(void) const;
		float					getDeltaTime(void) const;

		void	create_resizable_window(std::string const &name, int const major,
					int const minor, int const w, int const h);
		void	destroy_window(void);
		void	init_input_callback(void);
		void	update_events(void);
		void	swap_buffers(void);
		bool	should_window_be_closed(void);
		void	update_title_fps(void);
		void	update_title(std::string const &name);

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

	private :

		Glfw_manager(Glfw_manager const &src);
		Glfw_manager		&operator=(Glfw_manager const &rhs);

		Input			_input;
		Window			_window;
		std::string		_win_name;
		bool			_mouse_exclusive;
		float			_delta_time;
		float			_last_time;

		void	toogle_mouse_exclusive(void);

		static size_t	_nb_active_win;
};

#endif
