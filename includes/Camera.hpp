/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 16:13:19 by cledant           #+#    #+#             */
/*   Updated: 2017/09/05 12:18:34 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_HPP
# define CAMERA_HPP

# include "Input.hpp"
# include "GeneralException.hpp"
# include "glm/glm.hpp"
# include <iostream>

class Camera
{
	public :

		Camera(Input const &input, glm::vec3 const &pos,
			glm::vec3 const &world_up, glm::vec3 const &front, GLfloat yaw,
			GLfloat pitch);
		virtual ~Camera(void);

		void				update(float delta_time);
		void				toggle_update(void);
		glm::mat4 const		&getViewMatrix(void) const;


	class CameraFailException : public GeneralException
	{
		public :

			explicit CameraFailException(void);
			virtual ~CameraFailException(void) throw();
	};

	private :

		Input const		&_input;
		glm::vec3		_world_up;
		glm::vec3		_pos;
		glm::vec3		_front;
		glm::vec3		_up;
		glm::vec3		_right;
		glm::mat4		_view;
		GLfloat			_mouse_sensitivity;
		GLfloat			_movement_speed;
		bool			_update_cam;
		GLfloat			_yaw;
		GLfloat			_pitch;

		void			update_from_keyboard_input(float delta_time);
		void			update_from_mouse_input(void);
		void			update_vector_matrix(void);

		Camera(Camera const &src);
		Camera		&operator=(Camera const &rhs);
};

#endif
