/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/02 12:14:31 by cledant           #+#    #+#             */
/*   Updated: 2017/08/29 17:38:44 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Glfw_manager.hpp"

int		main(int argc, char **argv)
{
	Glfw_manager	manager;

	static_cast<void>(argc);
	static_cast<void>(argv);
	try
	{
		Glfw_manager::run_manager();
		manager.create_resizable_window("Particle System", 4, 1, 800, 600);
		manager.init_input_callback();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return (0);
	}
	while (Glfw_manager::getActiveWindowNumber() != 0)
	{
		if (manager.getWindow().win != nullptr)
		{
			manager.update_events();
			manager.swap_buffers();
			if (manager.should_window_be_closed() == true)
				manager.destroy_window();
		}
	}
	Glfw_manager::close_manager();
	return (0);
}
