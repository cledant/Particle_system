/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oGL_module.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/31 15:03:35 by cledant           #+#    #+#             */
/*   Updated: 2017/09/01 16:31:27 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OGL_MODULE_HPP
# define OGL_MODULE_HPP

# include "glfw3.h"
# include "GeneralException.hpp"
# include <iostream>
# include <vector>
# include <fstream>

class oGL_module
{
	public :

		oGL_module(void);
		virtual ~oGL_module(void);

		static void			oGL_check_error(void);
		static GLuint		oGL_create_vbo(size_t size);
		static void			oGL_delete_vbo(GLuint vbo);
//		void				oGL_add_to_vbo_list(GLuint vbo);
//		void				oGL_delete_from_vbo_list(GLuint vbo);
//		void				oGL_draw(void);

	class oGLFailException : public GeneralException
	{
		public :

			explicit oGLFailException(void);
			virtual ~oGLFailException(void) throw();
	};

	private :

		oGL_module(oGL_module const &src);
		oGL_module		&operator=(oGL_module const &rhs);

		static void		read_file(std::string const &path, std::string &content);
				
		std::vector<GLuint>		_gl_vbo_list;
};

#endif
