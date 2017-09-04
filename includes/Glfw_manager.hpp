/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Glfw_manager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/02 13:21:30 by cledant           #+#    #+#             */
/*   Updated: 2017/09/04 18:24:40 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLFW_MANAGER_HPP
# define GLFW_MANAGER_HPP

# include "glfw3.h"
# include "Input.hpp"
# include "Window.hpp"
# include "GeneralException.hpp"
# include <iostream>

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

		void	create_resizable_window(std::string const name, int const major,
					int const minor, int const w, int const h);
		void	destroy_window(void);
		void	init_input_callback(void);
		void	update_events(void);
		void	swap_buffers(void);
		bool	should_window_be_closed(void);

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

		static size_t	_nb_active_win;
};

#endif
