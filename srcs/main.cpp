/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/02 12:14:31 by cledant           #+#    #+#             */
/*   Updated: 2017/08/03 19:01:51 by cledant          ###   ########.fr       */
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
		manager.run_glfw();
		manager.create_ResizableWindow("Particle System", 4, 1, 800, 600);
		manager.init_input_callback();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return (0);
	}
	while (!glfwWindowShouldClose(manager.getWindow().win))
	{
		glfwGetTime();
	}
	return (0);
}
