/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Input.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/03 12:08:57 by cledant           #+#    #+#             */
/*   Updated: 2017/09/07 14:23:11 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_HPP
# define INPUT_HPP

# include <vector>
# include "glfw3.h"

# define PRESSED true
# define RELEASED false

struct		Input
{
	Input(void);
	~Input(void);

	std::vector<bool>	p_key;
	std::vector<bool>	p_mouse;
	GLfloat				pos_x;
	GLfloat				pos_y;
	GLfloat				last_pos_x;
	GLfloat				last_pos_y;
	float				timer;
};

#endif
