/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Window.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/03 11:39:23 by cledant           #+#    #+#             */
/*   Updated: 2017/08/03 11:51:55 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Window.hpp"

Window::Window(void)
{
	this->win = NULL;
	this->cur_win_w = MIN_WIN_W;
	this->cur_win_h = MIN_WIN_H;
	this->min_win_w = MIN_WIN_W;
	this->min_win_h = MIN_WIN_H;
	this->max_win_w = MAX_WIN_W;
	this->max_win_h = MAX_WIN_H;
}

Window::~Window(void)
{
}
