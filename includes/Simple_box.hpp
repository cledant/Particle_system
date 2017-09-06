/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Simple_box.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 11:44:29 by cledant           #+#    #+#             */
/*   Updated: 2017/09/06 16:16:47 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIMPLE_BOX_HPP
# define SIMPLE_BOX_HPP

#include "IEntity.hpp"
#include "oGL_module.hpp"
#include "Camera.hpp"
#include "glm/glm.hpp"

class Simple_box : public IEntity
{
	public :

		Simple_box(Shader const *shader, glm::mat4 const *perspective,
			Camera const *camera, glm::vec3 pos, glm::vec3 scale);
		virtual ~Simple_box(void);

		void				update(float time);
		void				draw(void);
		void				setPosition(glm::vec3 const &pos);
		void				setScale(glm::vec3 const &scale);
		glm::mat4 const		&getTotalMatrix(void);

	private :

		Shader const		*_shader;
		glm::mat4 const		*_perspective;
		Camera const		*_cam;
		GLuint				_vbo;
		GLuint				_vao;
		glm::vec3			_pos;
		glm::mat4			_model;
		glm::mat4			_total;
		glm::vec3			_scale;

		static float		_verticies[];
		static size_t		_nb_faces;

		Simple_box(Simple_box const &src);
		Simple_box	&operator=(Simple_box const &rhs);
};

#endif
