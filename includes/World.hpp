/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   World.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/31 15:03:35 by cledant           #+#    #+#             */
/*   Updated: 2017/09/23 17:30:23 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORLD_HPP
# define WORLD_HPP

# include "GeneralException.hpp"
# include "glm/glm.hpp"
# include "glm/gtc/matrix_transform.hpp"
# include "IEntity.hpp"
# include "IInteractive.hpp"
# include "Simple_box.hpp"
# include "Simple_cloud.hpp"
# include "Cubemap.hpp"
# include "oGL_module.hpp"
# include "oCL_module.hpp"
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

		void		update(void);
		void		render(void);
		IEntity		*add_Simple_box(Shader const *shader, glm::vec3 const &pos,
						glm::vec3 const &scale);
		IEntity		*add_Cubemap(Shader const *shader, Texture const *texture,
						glm::vec3 const &pos, glm::vec3 const &scale);
		IEntity		*add_Simple_cloud(Simple_cloud::Params &init);
		void		setActiveInteractive(IInteractive *ptr);
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
		IInteractive				*_active;
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
		float						_input_timer;
		float						_input_mouse_timer;

		World		&operator=(World const &rhs);
};

#endif
