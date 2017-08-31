/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/02 12:14:31 by cledant           #+#    #+#             */
/*   Updated: 2017/08/31 11:22:16 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Glfw_manager.hpp"
#include "Renderer.hpp"

int		main(int argc, char **argv)
{
	Glfw_manager	manager;
	Renderer		renderer;

	static_cast<void>(argc);
	static_cast<void>(argv);
	try
	{
		Glfw_manager::run_manager();
		manager.create_resizable_window("Particle System", 4, 1, 800, 600);
		manager.init_input_callback();
		renderer.oCL_init();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return (0);
	}
	while (Glfw_manager::getActiveWindowNumber())
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
