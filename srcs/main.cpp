/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/02 12:14:31 by cledant           #+#    #+#             */
/*   Updated: 2017/08/03 17:01:53 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Glfw_manager.hpp"

int		main(int argc, char **argv)
{
	Gflw_manager	manager;

	try
	{
		manager->run_glfw();
		manager->create_ResizableWindow("Particle System", 4, 1, 800, 600);
		manager->init_input_callback();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return (0);
	}
	while (!glfwWindowShouldClose(manager->getWindow()->win))
	{
		glfwGetTime();
	}
	return (0);
}
