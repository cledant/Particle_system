/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oGL_module.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/31 15:03:35 by cledant           #+#    #+#             */
/*   Updated: 2017/09/04 13:19:48 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OGL_MODULE_HPP
# define OGL_MODULE_HPP

# include "glfw3.h"
# include "Shader.hpp"
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

		void				add_shader(std::string const &name,
								std::string const &vs_path,
								std::string const &fs_path);
		Shader const		&getShader(std::string const &name);

	class ShaderNotFoundException : public GeneralException
	{
		public :

			explicit ShaderNotFoundException(void);
			explicit ShaderNotFoundException(std::string const &name);
			virtual ~ShaderNotFoundException(void) throw();
	};

	class oGLFailException : public GeneralException
	{
		public :

			explicit oGLFailException(void);
			virtual ~oGLFailException(void) throw();
	};

	private :

		std::vector<Shader>			_shader_list;

		oGL_module(oGL_module const &src);
		oGL_module		&operator=(oGL_module const &rhs);

		void			delete_all_shaders(void);
};

#endif
