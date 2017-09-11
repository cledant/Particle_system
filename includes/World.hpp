/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   World.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/31 15:03:35 by cledant           #+#    #+#             */
/*   Updated: 2017/09/11 12:53:37 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORLD_HPP
# define WORLD_HPP

# include "GeneralException.hpp"
# include "glm/glm.hpp"
# include "glm/gtc/matrix_transform.hpp"
# include "IEntity.hpp"
# include "Simple_box.hpp"
# include "Cubemap.hpp"
# include "oGL_module.hpp"
# include "Shader.hpp"
# include "Camera.hpp"
# include "Window.hpp"
# include "Input.hpp"
# include <iostream>
# include <vector>
# include <fstream>

class World
{
	public :

		World(Input const &input, Window const &win, glm::vec3 cam_pos,
			float max_fps, size_t max_frame_skip);
		virtual ~World(void);

		void		update(bool should_update_camera);
		void		render(void);
		void		add_Simple_box(Shader const *shader, glm::vec3 const &pos,
						glm::vec3 const &scale);
		void		add_Cubemap(Shader const *shader, Texture const *texture,
						glm::vec3 const &pos, glm::vec3 const &scale);
		void		updatePerspective(float fov);
		void		reset_update_timer(float time);
		void		reset_skip_loop(void);
		bool		should_be_updated(float time);

	class WorldFailException : public GeneralException
	{
		public :

			explicit WorldFailException(void);
			virtual ~WorldFailException(void) throw();
	};

	private :

		std::vector<IEntity *>		_entity_list;
		Input const					&_input;
		Window const				&_window;
		glm::mat4					_perspective;
		Camera						_camera;
		glm::mat4					_perspec_mult_view;
		float						_fov;
		float						_max_fps;
		size_t						_max_frame_skip;
		float						_tick;
		float						_next_update_tick;
		float						_last_update_tick;
		float						_delta_tick;
		size_t						_skip_loop;


		World		&operator=(World const &rhs);
};

#endif
