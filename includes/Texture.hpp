/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Texture.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/09 17:05:36 by cledant           #+#    #+#             */
/*   Updated: 2017/09/09 17:46:05 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURE_HPP
# define TEXTURE_HPP

# define GLFW_INCLUDE_GLCOREARB

# include "glfw3.h"
# include "GeneralException.hpp"
# include "glm/glm.hpp"
# include <iostream>
# include <fstream>
# include <vector>

class Texture
{
	public :

		Texture(std::string const &name, std::vector<std::string> const &files,
			t_tex_type type);
		Texture(Texture const &src);
		Texture		&operator=(Texture const &rhs);
		virtual ~Texture(void);

		std::string const		&getName(void) const;
		GLuint					getTextureID(void) const;

	typedef enum			e_tex_type
	{
		TEX_FLAT,
		TEX_CUBE,
	}						t_tex_type;

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

	class TypeException : public GeneralException
	{
		public :

			explicit TypeException(void);
			virtual ~TypeException(void) throw();
	};

	private :

		std::string			_name;
		GLuint				_tex_id;

		static GLuint		load_cubemap(std::vector<std::string> const &files);
};

#endif
