/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Simple_cloud.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/31 15:03:35 by cledant           #+#    #+#             */
/*   Updated: 2017/09/12 13:53:24 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIMPLE_CLOUD_HPP
# define SIMPLE_CLOUD_HPP

# include "glfw3.h"
# include "glm/glm.hpp"
# include "oCL_module.hpp"
# include "oGL_module.hpp"
# include "Shader.hpp"
# include "GeneralException.hpp"
# include <iostream>
# include <vector>
# include <fstream>

class Simple_cloud : public IEntity
{
	public :

		Simple_cloud(size_t nb_particule, cl::Context const &cl_context,
				cl::Program const &cl_prog, Shader const &shader);
		virtual ~Simple_cloud(void);

		void					update(float time);
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

		Shader const				*_shader;
		cl::Context const			*_cl_context;
		cl::CommandQueue const		*_cl_cq;
		cl::Kernel const			*_cl_kernel_random;
		cl::Kernel const			*_cl_kernel_gravity;
		bool						_update_positions;
		size_t						_nb_particle;
		glm::vec3					_gravity_center;
		GLuint						_gl_vbo;
		GLuint						_gl_vao;
		cl::BufferGL				_cl_vbo;

		Simple_cloud(Simple_cloud const &src);
		Simple_cloud	&operator=(Simple_cloud const &rhs);

		void			set_kernel_args(void);
};

#endif
