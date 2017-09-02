/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oCL_module.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/30 13:55:38 by cledant           #+#    #+#             */
/*   Updated: 2017/09/02 15:58:19 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OCL_MODULE_HPP
# define OCL_MODULE_HPP

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
# define __CL_ENABLE_EXCEPTION

class oCL_module
{
	public :

		oCL_module(void);
		virtual ~oCL_module(void);

		static void				oCL_check_error(cl_int const err, cl_int const ref);
		static void				oCL_create_cl_vbo(GLuint gl_vbo,
									cl::Context const &context,
									cl::BufferGL &new_buff);
		static void				oCL_create_kernel(std::string const &name,
									cl::Program const &program,
									cl::Kernel &kernel);

		void					oCL_init(void);
		void					oCL_add_code(std::string const &file);
		void					oCL_compile_program(void);
		void					oCL_run_kernel_oGL_buffer(GLuint gl_vbo,
									cl::BufferGL const &cl_vbo,
									cl::Kernel const &kernel, size_t worksize);

		cl::Context const		&getContext(void) const;
		cl::Program const		&getProgram(void) const;

	class oCLFailException : public GeneralException
	{
		public :

			explicit oCLFailException(void);
			virtual ~oCLFailException(void) throw();
	};

	private :

		oCL_module(oCL_module const &src);
		oCL_module		&operator=(oCL_module const &rhs);

		void			oCL_get_platform_list(void);
		void			oCL_get_device_list(cl_device_type type);
		bool			oCL_select_platform_from_name(std::string const &name);
		void			oCL_select_first_oGL_sharing_device(void);
		void			oCL_create_context(void);
		void			oCL_create_command_queue(void);

		static void		read_file(std::string const &path, std::string &content);

		std::vector<cl::Platform>	_cl_platform_list;
		std::vector<cl::Device>		_cl_device_list;
		cl::Platform				_cl_platform;
		cl::Device					_cl_device;
		cl::Context					_cl_context;
		cl::CommandQueue			_cl_cc;
		cl::Program::Sources		_cl_sources;
		cl::Program					_cl_program;
		cl::Event					_cl_event;
};

#endif
