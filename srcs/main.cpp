/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/02 12:14:31 by cledant           #+#    #+#             */
/*   Updated: 2017/09/04 15:48:34 by cledant          ###   ########.fr       */
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
	World			*world;
//	Simple_cloud	*sc;

	static_cast<void>(argc);
	static_cast<void>(argv);
	try
	{
		Glfw_manager::run_manager();
		manager.create_resizable_window("Particle System", 4, 1, 800, 600);
		manager.init_input_callback();
		manager.add_shader("simple_box", "./shaders/simple_box.vs",
			"./shaders/simple_box.fs");
		world = new World(manager.getInput(), manager.getWindow());
/*		oCL.oCL_init();
		oCL.oCL_add_code(Simple_cloud::kernel_path);
		oCL.oCL_compile_program();
		sc = new Simple_cloud(1000000, oCL.getContext(), oCL.getProgram());*/
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
/*			oCL.oCL_run_kernel_oGL_buffer(sc->get_gl_vbo(), sc->get_cl_vbo(),
				sc->get_cl_kernel(), sc->get_nb_particle());*/
			manager.swap_buffers();
			if (manager.should_window_be_closed() == true)
				manager.destroy_window();
		}
	}
//	delete sc;
	delete world;
	oGL.delete_all_shaders();
	Glfw_manager::close_manager();
	return (0);
}
