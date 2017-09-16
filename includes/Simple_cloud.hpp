/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Simple_cloud.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/31 15:03:35 by cledant           #+#    #+#             */
/*   Updated: 2017/09/16 16:34:51 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIMPLE_CLOUD_HPP
# define SIMPLE_CLOUD_HPP

# include "glfw3.h"
# include "glm/glm.hpp"
# include "glm/gtc/matrix_transform.hpp"
# include "oCL_module.hpp"
# include "oGL_module.hpp"
# include "Shader.hpp"
# include "IEntity.hpp"
# include "IInteractive.hpp"
# include "GeneralException.hpp"
# include <iostream>
# include <vector>
# include <random>

class Simple_cloud : public IEntity, public IInteractive
{
	public :

		Simple_cloud(size_t nb_particle, cl::Context const *context,
				glm::vec3 const &pos, Shader const *shader,
				cl::CommandQueue const *cq, cl::Kernel const *random,
				cl::Kernel const *gravity, glm::mat4 const *perspec_mult_view);
		virtual ~Simple_cloud(void);

		void					update(float time);
		void					update_interaction(Input const &input);
		void					draw(void);

		void					setPosition(glm::vec3 const &pos);
		glm::mat4 const			&getTotalMatrix(void) const;

	class Simple_cloudFailException : public GeneralException
	{
		public :

			explicit Simple_cloudFailException(void);
			virtual ~Simple_cloudFailException(void) throw();
	};

	typedef struct					s_particle
	{
		glm::vec4					pos;
		glm::vec4					vel;
		glm::vec4					acc;
		glm::vec4					lifetime;
	}								t_particle;

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
		glm::mat4					_total;
		GLuint						_gl_vbo;
		GLuint						_gl_vao;
		cl::BufferGL				_cl_vbo;
		std::random_device			_rd;
		float						_particle_mass;
		float						_center_mass;

		Simple_cloud(Simple_cloud const &src);
		Simple_cloud	&operator=(Simple_cloud const &rhs);

		void					_generate_random_uint2(unsigned int (*random)[2]);
		void					_set_random_kernel_args(void);
		void					_set_gravity_kernel_args(float time);
};

#endif
