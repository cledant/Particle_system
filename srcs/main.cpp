/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/02 12:14:31 by cledant           #+#    #+#             */
/*   Updated: 2017/09/02 12:47:16 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Glfw_manager.hpp"
#include "oCL_module.hpp"
#include "oGL_module.hpp"
#include "Simple_cloud.hpp"

int		main(int argc, char **argv)
{
	Glfw_manager	manager;
	oCL_module		oCL;
	oGL_module		oGL;
	Simple_cloud	*sc;

	static_cast<void>(argc);
	static_cast<void>(argv);
	try
	{
		Glfw_manager::run_manager();
		manager.create_resizable_window("Particle System", 4, 1, 800, 600);
		manager.init_input_callback();
		oCL.oCL_init();
		oCL.add_code(Simple_cloud::kernel_path);
		oCL.compile_program();
		sc = Simple_cloud::Simple_cloud(1000000, &(oCL.getContext()));
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
	delete sc;
	Glfw_manager::close_manager();
	return (0);
}
