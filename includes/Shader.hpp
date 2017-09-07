/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shader.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 13:11:10 by cledant           #+#    #+#             */
/*   Updated: 2017/09/07 10:06:41 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHADER_HPP
# define SHADER_HPP

# define GLFW_INCLUDE_GLCOREARB

# include "glfw3.h"
# include "GeneralException.hpp"
# include "glm/glm.hpp"
# include <iostream>
# include <fstream>

class Shader
{
	public :

		Shader(std::string const &name, std::string const &vs_path,
				std::string const &fs_path);
		Shader(Shader const &src);
		Shader		&operator=(Shader const &rhs);
		virtual ~Shader(void);

		std::string const		&getName(void) const;
		GLuint					getShaderProgram(void) const;
		void					use(void) const;
		void					setMat4(GLint uniform_id,
									glm::mat4 const &mat4) const;

	class FileOpenException : public GeneralException
	{
		public :

			explicit FileOpenException(void);
			explicit FileOpenException(std::string const &path);
			virtual ~FileOpenException(void) throw();
	};

	class AllocationException : public GeneralException
	{
		public :

			explicit AllocationException(void);
			virtual ~AllocationException(void) throw();
	};

	class CompileException : public GeneralException
	{
		public :

			explicit CompileException(void);
			virtual ~CompileException(void) throw();
	};

	class LinkException : public GeneralException
	{
		public :

			explicit LinkException(void);
			virtual ~LinkException(void) throw();
	};

	private :

		std::string			_name;
		GLuint				_shader_program;

		static GLuint		load_shader(std::string const &path, GLenum type);
		static GLuint		compile_program(GLuint vs, GLuint fs);
		static void			get_shader_error(GLuint shader);
		static void			read_file(std::string const &path, std::string &content);
};

#endif
