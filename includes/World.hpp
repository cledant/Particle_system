/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   World.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/31 15:03:35 by cledant           #+#    #+#             */
/*   Updated: 2017/09/05 10:33:43 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORLD_HPP
# define WORLD_HPP

# include "GeneralException.hpp"
# include "glm/glm.hpp"
# include <iostream>
# include <vector>
# include <fstream>

class World
{
	public :

		World(Input const &input, Window const &win);
		virtual ~World(void);

		void		update(float time);
		void		render(void);
		void		add_Simple_box(void);
		float		getDeltaTime(void);

	class WorldFailException : public GeneralException
	{
		public :

			explicit WorldFailException(void);
			virtual ~WorldFailException(void) throw();
	};

	private :

		std::vector<IEntity>		_entity_list;
		Input const					&_input;
		Window const				&_window;
		glm::mat4					_perspective;
		Camera						_camera;
		float						delta_time;
		float						last_time;

		World(World const &src);
		World		&operator=(World const &rhs);
};

#endif
