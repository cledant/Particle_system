/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Window.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/02 17:19:31 by cledant           #+#    #+#             */
/*   Updated: 2017/08/29 17:08:49 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WINDOW_HPP
# define WINDOW_HPP

# include "glfw3.h"

# define MAX_WIN_W	1980
# define MAX_WIN_H	1080
# define MIN_WIN_W	680
# define MIN_WIN_H	480

struct	Window
{
	Window(void);
	~Window(void);

	GLFWwindow		*win;
	int				cur_win_w;
	int				cur_win_h;
	int				max_win_w;
	int				max_win_h;
	int				min_win_w;
	int				min_win_h;
};

#endif
