/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/02 12:14:31 by cledant           #+#    #+#             */
/*   Updated: 2017/09/20 19:49:02 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Glfw_manager.hpp"
#include "oGL_module.hpp"
#include "oCL_module.hpp"
#include "World.hpp"

#define MAX_PARTICLE 8000000

static void				print_instruction(void)
{
	std::string const			particle_number_line
	{
		"Usage : ./particle_system _nbr_particles_\n"
		"Particle number : 1 to "
	};

	std::string const			instruction
	{
		"W = FORWARD\n"
		"S = BACKWARD\n"
		"A = LEFT\n"
		"D = RIGHT\n"
		"E = UP\n"
		"Q =  DOWN\n"
		"P = Turn on/off gravity refresh\n"
		"R = Reset particle and gravity center and change random type"
	};

	std::cout << particle_number_line << MAX_PARTICLE << std::endl;
	std::cout << instruction << std::endl;
}

static void				main_loop(World &world, Glfw_manager &manager)
{
	while (Glfw_manager::getActiveWindowNumber())
	{
		if (manager.getWindow().win != nullptr)
		{
			world.reset_skip_loop();
			while (world.should_be_updated(Glfw_manager::getTime()) == true)
			{
				manager.update_events();
				world.update();
			}
			world.render();
			manager.swap_buffers();
			manager.calculate_and_display_fps();
			if (manager.should_window_be_closed() == true)
				manager.destroy_window();
		}
	}
	oGL_module::oGL_finish();
}

static void				init_oGL(oGL_module &oGL)
{
	std::vector<std::string> const				tex_files
	{
		"./textures/skybox/right.jpg",
		"./textures/skybox/left.jpg",
		"./textures/skybox/top.jpg",
		"./textures/skybox/bottom.jpg",
		"./textures/skybox/back.jpg",
		"./textures/skybox/front.jpg",
	};

	oGL_module::oGL_enable_depth();
	oGL.add_shader("simple_box", "./shaders/simple_box/simple_box.vs",
		"./shaders/simple_box/simple_box.fs");
	oGL.add_shader("cubemap", "./shaders/cubemap/cubemap.vs",
		"./shaders/cubemap/cubemap.fs");
	oGL.add_shader("simple_cloud", "./shaders/simple_cloud/simple_cloud.vs",
		"./shaders/simple_cloud/simple_cloud.fs");
	oGL.add_texture("skybox", tex_files, Texture::TEX_CUBE);
}

static void				init_oCL(oCL_module &oCL)
{
	oCL.init();
	oCL.add_code("./kernels/particle.clh");
	oCL.add_code("./kernels/random/random_hat.cl");
	oCL.add_code("./kernels/random/random_disc.cl");
	oCL.add_code("./kernels/random/random_sphere.cl");
	oCL.add_code("./kernels/random/random_cute.cl");
	oCL.add_code("./kernels/random/random_square.cl");
	oCL.add_code("./kernels/random/random_cross.cl");
	oCL.add_code("./kernels/gravity/gravity.cl");
	oCL.compile_program();
	oCL.create_kernel("random_hat");
	oCL.create_kernel("random_disc");
	oCL.create_kernel("random_sphere");
	oCL.create_kernel("random_cute");
	oCL.create_kernel("random_square");
	oCL.create_kernel("random_cross");
	oCL.create_kernel("gravity");
}

static void				init_program(World **world, oGL_module &oGL, oCL_module &oCL,
							Glfw_manager &manager, size_t nb_particle)
{


	Glfw_manager::run_manager();
	manager.create_resizable_window("Particle System", 4, 1, 1000, 1000);
	manager.init_input_callback();
	init_oCL(oCL);
	init_oGL(oGL);
	(*world) = new World(manager.getInput(), manager.getWindow(),
			glm::vec3(0.0f, 0.0f, 10.0f), 60.0f, 10);
	(*world)->add_Cubemap(&(oGL.getShader("cubemap")), &(oGL.getTexture("skybox")),
			glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(100.0f, 100.0f, 100.0f));
	(*world)->setActiveInteractive(dynamic_cast<IInteractive *>(
			(*world)->add_Simple_cloud(nb_particle, &(oCL.getContext()),
			glm::vec3(0.0f, 0.0f, 0.0f), &(oGL.getShader("simple_cloud")),
			&(oCL.getCommandQueue()),
			std::vector<cl::Kernel const *>{&(oCL.getKernel("random_square")),
			&(oCL.getKernel("random_sphere")), &(oCL.getKernel("random_disc")),
			&(oCL.getKernel("random_hat")), &(oCL.getKernel("random_cross")),
			&(oCL.getKernel("random_cute"))},
			&(oCL.getKernel("gravity")))));
}

static void				run_program(size_t nb_particle)
{
	Glfw_manager								manager;
	oGL_module									oGL;
	oCL_module									oCL;
	World										*world = nullptr;

	try
	{
		init_program(&world, oGL, oCL, manager, nb_particle);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		delete world;
		oGL.delete_all_shaders();
		oGL.delete_all_textures();
		Glfw_manager::close_manager();
		return ;
	}
	world->reset_update_timer(Glfw_manager::getTime());
	manager.reset_fps_counter();
	manager.toogle_mouse_exclusive();
	main_loop(*world, manager);
	std::cout << "Delete world" << std::endl;
	delete world;
	std::cout << "Delete shaders" << std::endl;
	oGL.delete_all_shaders();
	std::cout << "Delete textures" << std::endl;
	oGL.delete_all_textures();
	std::cout << "Close manager" << std::endl;
	Glfw_manager::close_manager();
}

static size_t			check_argv(std::string const &str)
{
	size_t		nb_particle = 0;

	try
	{
		nb_particle = std::stoul(str);
	}
	catch (std::exception &e)
	{
		std::cout << "Particle system : Invalid argument" << std::endl;
		return (0);
	}
	if (nb_particle > MAX_PARTICLE)
	{
		std::cout << "Particle system : Number of particle too big" << std::endl;
		return (0);
	}
	return (nb_particle);
}

int						main(int argc, char **argv)
{
	size_t		nb_particle = 0;

	if (argc == 2)
	{
		if ((nb_particle = check_argv(argv[1])) == 0)
		{
			print_instruction();
			return (0);
		}
		run_program(nb_particle);
	}
	else
		print_instruction();
	return (0);
}
