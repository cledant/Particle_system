/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/02 12:14:31 by cledant           #+#    #+#             */
/*   Updated: 2017/09/11 11:09:12 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Glfw_manager.hpp"
#include "oGL_module.hpp"
#include "World.hpp"

int		main(int argc, char **argv)
{
	Glfw_manager				manager;
	oGL_module					oGL;
	World						*world = nullptr;
	std::vector<std::string>	tex_files
	{
		"./textures/skybox/right.jpg",
		"./textures/skybox/left.jpg",
		"./textures/skybox/top.jpg",
		"./textures/skybox/bottom.jpg",
		"./textures/skybox/back.jpg",
		"./textures/skybox/front.jpg",
	};

	static_cast<void>(argc);
	static_cast<void>(argv);
	try
	{
		Glfw_manager::run_manager();
		manager.create_resizable_window("Particle System", 4, 1, 1000, 1000);
		manager.init_input_callback();
		oGL_module::oGL_enable_depth();
		oGL.add_shader("simple_box", "./shaders/simple_box/simple_box.vs",
			"./shaders/simple_box/simple_box.fs");
		oGL.add_shader("cubemap", "./shaders/cubemap/cubemap.vs",
			"./shaders/cubemap/cubemap.fs");
		oGL.add_texture("skybox", tex_files, Texture::TEX_CUBE);
		world = new World(manager.getInput(), manager.getWindow(),
				glm::vec3(0.0f, 0.0f, 10.0f));
		world->add_Cubemap(&(oGL.getShader("cubemap")), &(oGL.getTexture("skybox")),
				glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(20.0f, 20.0f, 20.0f));
		world->add_Simple_box(&(oGL.getShader("simple_box")),
				glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		world->add_Simple_box(&(oGL.getShader("simple_box")),
				glm::vec3(3.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		world->add_Simple_box(&(oGL.getShader("simple_box")),
				glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		world->add_Simple_box(&(oGL.getShader("simple_box")),
				glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		world->add_Simple_box(&(oGL.getShader("simple_box")),
				glm::vec3(0.0f, -3.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		world->add_Simple_box(&(oGL.getShader("simple_box")),
				glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		world->add_Simple_box(&(oGL.getShader("simple_box")),
				glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		delete world;
		oGL.delete_all_shaders();
		oGL.delete_all_textures();
		Glfw_manager::close_manager();
		return (0);
	}

	float next_tick = 0.0f;
	float game_tick = 0.0f;
	float last_game_tick = 0.0f;
	float max_fps = 60.0f;
	float tick = 1.0f / max_fps;
	size_t	loop = 0;
	size_t	max_frameskip = 10;

	next_tick = Glfw_manager::getTime();

	manager.reset_fps_counter();
	while (Glfw_manager::getActiveWindowNumber())
	{
		if (manager.getWindow().win != nullptr)
		{
			loop = 0;
			if ((game_tick = Glfw_manager::getTime()) > next_tick &&
					loop < max_frameskip)
			{
				manager.update_events();
				world->update(game_tick - last_game_tick, manager.getMouseMode());
				loop++;
				next_tick += tick;
				last_game_tick = game_tick;
			}

			world->render();
			manager.swap_buffers();
			manager.calculate_and_display_fps();
			if (manager.should_window_be_closed() == true)
				manager.destroy_window();
		}
	}
	delete world;
	oGL.delete_all_shaders();
	oGL.delete_all_textures();
	Glfw_manager::close_manager();
	return (0);
}
