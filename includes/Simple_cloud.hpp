/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Simple_cloud.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/31 15:03:35 by cledant           #+#    #+#             */
/*   Updated: 2017/09/06 15:02:18 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIMPLE_CLOUD_HPP
# define SIMPLE_CLOUD_HPP

# include "glfw3.h"
# include "oCL_module.hpp"
# include "oGL_module.hpp"
# include "Shader.hpp"
# include "GeneralException.hpp"
# include <iostream>
# include <vector>
# include <fstream>

class Simple_cloud
{
	public :

		Simple_cloud(size_t nb_particule, cl::Context const &cl_context,
				cl::Program const &cl_prog, Shader const &shader);
		virtual ~Simple_cloud(void);

		void					draw(void);

		GLuint					get_gl_vbo(void) const;
		cl::BufferGL const		&get_cl_vbo(void) const;
		cl::Context const		&get_cl_context(void) const;
		size_t					get_nb_particle(void) const;
		cl::Kernel const		&get_cl_kernel(void) const;
		cl::Program const		&get_cl_program(void) const;

		static std::string const	kernel_name;
		static std::string const	kernel_path;
		static std::string const	vs;
		static std::string const	fs;

	class Simple_cloudFailException : public GeneralException
	{
		public :

			explicit Simple_cloudFailException(void);
			virtual ~Simple_cloudFailException(void) throw();
	};

	private :

		cl::Context const			&_cl_context;
		cl::Program const			&_cl_program;
		size_t						_nb_particle;
		GLuint						_gl_vbo;
		cl::BufferGL				_cl_vbo;
		cl::Kernel					_cl_kernel;
		Shader const				&_shader;

		Simple_cloud(Simple_cloud const &src);
		Simple_cloud	&operator=(Simple_cloud const &rhs);

		void			set_kernel_args(void);
};

#endif
