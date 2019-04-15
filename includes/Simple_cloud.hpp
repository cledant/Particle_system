/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Simple_cloud.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/31 15:03:35 by cledant           #+#    #+#             */
/*   Updated: 2017/09/24 09:06:11 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIMPLE_CLOUD_HPP
# define SIMPLE_CLOUD_HPP

# include "oGL_module.hpp"
# include "glm/glm.hpp"
# include "glm/gtc/matrix_transform.hpp"
# include "oCL_module.hpp"
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

	typedef enum					e_gravity_control
	{
		MOUSE_FOLLOW = 0,
		MOUSE_CLICK = 1,
	}								t_gravity_control;

	typedef enum					e_click_type
	{
		GRAVITY_POS = 0,
		EMITTER_POS = 1,
	}								t_click_type;

	struct									Params
	{
		Params(void);
		~Params(void);

		size_t								nb_particle;
		cl::Context const					*context;
		glm::vec3							gravity_pos;
		glm::vec3							emitter_pos;
		Shader const						*shader;
		cl::CommandQueue const				*cq;
		std::vector<cl::Kernel const *>		vec_random;
		cl::Kernel const					*gravity;
		cl::Kernel const					*lifetime;
		glm::mat4 const						*perspec_mult_view;
		float								refresh_tick;
		float								min_random;
		float								max_random;
		float								min_lifetime;
		float								max_lifetime;
		unsigned int						color;
		bool								update_gravity;
		bool								update_lifetime;
		t_gravity_control					grav_ctrl_type;
		t_click_type						click_type;
	};

		Simple_cloud(Simple_cloud::Params const &init);
		virtual ~Simple_cloud(void);

		void					update(float time);
		bool					update_keyboard_interaction(Input const &input,
									float input_timer);
		bool					update_mouse_interaction(Input const &input,
									GLFW_Window const &win, glm::vec3 const &origin,
									std::vector<glm::vec3 const *> const &axes,
									float input_timer);
		void					draw(void);

		void					setPosition(glm::vec3 const &pos);
		glm::mat4 const			&getTotalMatrix(void) const;

	class Simple_cloudFailException : public GeneralException
	{
		public :

			explicit Simple_cloudFailException(void);
			virtual ~Simple_cloudFailException(void) throw();
	};

	private :

	typedef struct					s_particle
	{
		glm::vec4					pos;
		glm::vec4					vel;
		glm::vec4					acc;
		glm::vec4					lifetime;
	}								t_particle;

	typedef struct					s_rgba
	{
		char						r;
		char						g;
		char						b;
		char						a;
	}								t_rgba;

		Shader const							*_shader;
		cl::CommandQueue const					*_cl_cq;
		std::vector<cl::Kernel const *> const	_cl_vec_random_kernel;
		cl::Kernel const						*_cl_kernel_random;
		cl::Kernel const						*_cl_kernel_gravity;
		cl::Kernel const						*_cl_kernel_lifetime;
		glm::mat4 const							*_perspec_mult_view;
		size_t									_nb_particle;
		glm::vec3								_pos;
		glm::vec3								_mouse_3d_pos;
		glm::vec3								_emitter_pos;
		glm::mat4								_total;
		GLuint									_gl_vbo;
		GLuint									_gl_vao;
		std::vector<cl::Memory>					_cl_vbo;
		std::random_device						_rd;
		float									_particle_mass;
		float									_center_mass;
		float									_refresh_tick;
		size_t									_cur_random;
		t_gravity_control						_grav_ctrl_type;
		t_click_type							_click_type;
		unsigned int							_color;
		glm::vec3								_gl_color;
		float									_grav_mult;
		float									_min_random;
		float									_max_random;
		float									_min_lifetime;
		float									_max_lifetime;
		bool									_generate_random;
		bool									_update_gravity;
		bool									_update_lifetime;

		Simple_cloud(Simple_cloud const &src);
		Simple_cloud	&operator=(Simple_cloud const &rhs);

		void					_generate_random_uint2(unsigned int (*random)[2]);
		void					_set_random_kernel_args(void);
		void					_set_lifetime_kernel_args(void);
		void					_set_gravity_kernel_args(void);
		void					_right_shift_color(void);
		void					_left_shift_color(void);
		void					_convert_color_to_float_color(void);
		void					_compute_mouse_3d_pos(Input const &input,
									GLFW_Window const &win, glm::vec3 const &origin,
									std::vector<glm::vec3 const *> const &axes);
		void					_switch_gravity_mode(void);
		void					_switch_lifetime_mode(void);
		void					_switch_click_mode(void);
		void					_reset_and_switch_type(void);
};

#endif
