/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oCL_module.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/30 13:55:38 by cledant           #+#    #+#             */
/*   Updated: 2017/09/15 14:00:28 by cledant          ###   ########.fr       */
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

class oCL_module
{
	public :

		oCL_module(void);
		virtual ~oCL_module(void);

		static void				oCL_check_error(cl_int err, cl_int ref);
		static void				oCL_create_cl_vbo(GLuint gl_vbo,
									cl::Context const &context,
									cl::BufferGL &new_buff);
		static void				oCL_run_kernel_oGL_buffer(cl::BufferGL &cl_vbo,
									cl::Kernel &cl_kernel, cl::CommandQueue &cl_cq,
									size_t worksize);

		void					init(void);
		void					add_code(std::string const &file);
		void					compile_program(void);
		void					create_kernel(std::string const &name);

		cl::Context const		&getContext(void) const;
		cl::CommandQueue const	&getCommandQueue(void) const;
		cl::Kernel const		&getKernel(std::string const &name) const;

	class oCLFailException : public GeneralException
	{
		public :

			explicit oCLFailException(void);
			virtual ~oCLFailException(void) throw();
	};

	private :

	class FileOpenException : public GeneralException
	{
		public :

			explicit FileOpenException(std::string const &path);
			explicit FileOpenException(void);
			virtual ~FileOpenException(void) throw();
	};

	class KernelNotFoundException : public GeneralException
	{
		public :

			explicit KernelNotFoundException(std::string const &path);
			explicit KernelNotFoundException(void);
			virtual ~KernelNotFoundException(void) throw();
	};

		oCL_module(oCL_module const &src);
		oCL_module		&operator=(oCL_module const &rhs);

		void			_get_platform_list(void);
		void			_get_device_list(cl_device_type type);
		bool			_select_platform_from_name(std::string const &name);
		void			_select_first_oGL_sharing_device(void);
		void			_create_context(void);
		void			_create_command_queue(void);

		static void		_read_file(std::string const &path,
							std::string &content);

		std::vector<cl::Platform>	_cl_platform_list;
		std::vector<cl::Device>		_cl_device_list;
		cl::Platform				_cl_platform;
		cl::Device					_cl_device;
		cl::Context					_cl_context;
		cl::CommandQueue			_cl_cq;
		cl::Program::Sources		_cl_sources;
		cl::Program					_cl_program;
		std::vector<cl::Kernel>		_cl_kernel_list;
		std::vector<std::string>	_cl_str_sources;
};

#endif
