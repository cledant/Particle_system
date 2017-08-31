/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Renderer.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/30 13:55:38 by cledant           #+#    #+#             */
/*   Updated: 2017/08/31 12:33:19 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDERER_HPP
# define RENDERER_HPP

# include "glfw3.h"
# include "GeneralException.hpp"
# include "cl.hpp"
# include <iostream>
# include <string>
# include <vector>
# include <fstream>
# ifdef __APPLE__
	# include <OpenCL/cl_gl_ext.h>
	# include <OpenGL/CGLDevice.h>
	# include <OpenGL/CGLCurrent.h>
#endif

class Renderer
{
	public :

		Renderer(void);
		virtual ~Renderer(void);

		static void		oCL_check_error(cl_int const err, cl_int const ref);
		void			oCL_init(void);

	class oCLFailException : public GeneralException
	{
		public :

			explicit oCLFailException(void);
			virtual ~oCLFailException(void) throw();
	};

	private :

		Renderer(Renderer const &src);
		Renderer		&operator=(Renderer const &rhs);

		void			oCL_get_platform_list(void);
		void			oCL_get_device_list(cl_device_type type);
		bool			oCL_select_platform_from_name(std::string const &name);
		void			oCL_select_first_oGL_sharing_device(void);
		void			oCL_create_context(void);
		void			oCL_create_command_queue(void);
		void			oCL_add_code(std::string const &file);
		void			oCL_compile_program(void);
		void			oCL_create_kernel(std::string const &name);

		static void		read_file(std::string const &path, std::string &content);

		std::vector<cl::Platform>	_cl_platform_list;
		std::vector<cl::Device>		_cl_device_list;
		cl::Platform				_cl_platform;
		cl::Device					_cl_device;
		cl::Context					_cl_context;
		cl::CommandQueue			_cl_cc;
		cl::Program::Sources		_cl_sources;
		cl::Program					_cl_program;
		cl::Kernel					_cl_kernel;
};

#endif
