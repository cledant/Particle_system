/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   World.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/31 15:03:35 by cledant           #+#    #+#             */
/*   Updated: 2017/09/07 11:03:57 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORLD_HPP
# define WORLD_HPP

# include "GeneralException.hpp"
# include "glm/glm.hpp"
# include "glm/gtc/matrix_transform.hpp"
# include "IEntity.hpp"
# include "Simple_box.hpp"
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

		World(Input const &input, Window const &win, glm::vec3 cam_pos, float time);
		virtual ~World(void);

		void		update(float time);
		void		render(void);
		void		add_Simple_box(Shader const *shader, glm::vec3 const &pos,
						glm::vec3 const &scale);
		void		updatePerspective(float fov);
		float		getDeltaTime(void) const;

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
		float						_delta_time;
		float						_last_time;
		float						_fov;

		World		&operator=(World const &rhs);
};

#endif
