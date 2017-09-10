/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cubemap.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/09 16:07:59 by cledant           #+#    #+#             */
/*   Updated: 2017/09/10 12:07:12 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBEMAP_HPP
# define CUBEMAP_HPP

#include "IEntity.hpp"
#include "oGL_module.hpp"
#include "Camera.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Cubemap : public IEntity
{
	public :

		Cubemap(Shader const *shader, glm::mat4 const *perspective,
			Camera const *camera, Texture const *texture, glm::vec3 const &pos,
			glm::vec3 const &scale);
		virtual ~Cubemap(void);

		void				update(float time);
		void				draw(void);
		void				setScale(glm::vec3 const &scale);
		glm::mat4 const		&getTotalMatrix(void);

	class InitException : public GeneralException
	{
		public :

			explicit	InitException(void);
			virtual		~InitException(void) throw();
	};

	private :

		Shader const		*_shader;
		glm::mat4 const		*_perspective;
		Camera const		*_cam;
		Texture const		*_tex;
		GLuint				_vbo;
		GLuint				_vao;
		glm::vec3			_pos;
		glm::vec3			_scale;
		glm::mat4			_total;

		static float		_vertices[];
		static size_t		_nb_faces;

		Cubemap(Cubemap const &src);
		Cubemap		&operator=(Cubemap const &rhs);
};

#endif
