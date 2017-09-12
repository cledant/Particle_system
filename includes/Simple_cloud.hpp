/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Simple_cloud.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/31 15:03:35 by cledant           #+#    #+#             */
/*   Updated: 2017/09/12 19:20:03 by cledant          ###   ########.fr       */
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
# include <random>

class Simple_cloud : public IEntity
{
	public :

		Simple_cloud(size_t nb_particule, cl::Context const *context,
				glm::vec3 const &pos, glm::vec3 const &scale, Shader const *shader,
				cl::CommandQueue const *cq, cl::Kernel const *random,
				cl::kernel const *gravity, glm::mat4 const *perspec_mult_view);
		virtual ~Simple_cloud(void);

		void					update(float time);
		void					draw(void);

		void					setPosition(glm::vec3 const &pos);
		void					request_random(void);
		void					toogle_gravity_refresh(void);

		glm::mat4 const			&getTotalMatrix(void) const;

	class Simple_cloudFailException : public GeneralException
	{
		public :

			explicit Simple_cloudFailException(void);
			virtual ~Simple_cloudFailException(void) throw();
	};

	private :

		Shader const				*_shader;
		cl::CommandQueue const		*_cl_cq;
		cl::Kernel const			*_cl_kernel_random;
		cl::Kernel const			*_cl_kernel_gravity;
		glm::mat4 const				*_perspec_mult_view;
		bool						_generate_random;
		bool						_update_gravity;
		size_t						_nb_particle;
		glm::vec3					_pos;
		glm::vec3					_scale;
		glm::mat4					_total;
		GLuint						_gl_vbo;
		GLuint						_gl_vao;
		cl::BufferGL				_cl_vbo;
		std::random_device			_rd;

		Simple_cloud(Simple_cloud const &src);
		Simple_cloud	&operator=(Simple_cloud const &rhs);

		void			_set_random_kernel_args(void);
};

#endif
