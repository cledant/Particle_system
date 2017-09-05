/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Simple_box.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 11:44:29 by cledant           #+#    #+#             */
/*   Updated: 2017/09/05 16:17:57 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIMPLE_BOX_HPP
# define SIMPLE_BOX_HPP

#include "IEntity.hpp"
#include "oGL_module.hpp"
#include "glm/glm.hpp"

class Simple_box : public IEntity
{
	public :

		Simple_box(Shader const *shader, glm::mat4 const *perspective,
			Camera const *camera, glm::vec3 pos, glm::vec3 scale);
		virtual ~Simple_box(void);

		void		update(float time);
		void		draw(void);
		void		setPosition(glm::vec3 pos);

	private :

		Shader const		*_shader;
		glm::mat4 const		*_perspective;
		Camera const		*_cam;
		GLuint				_vbo;
		GLuint				_vao;
		glm::vec3			_pos;
		glm::mat4			_model;
		glm::vec3			_scale;

		static float		_verticies[];

		Simple_box(Simple_box const &src);
		Simple_box	&operator=(Simple_box const &rhs);
};

#endif
