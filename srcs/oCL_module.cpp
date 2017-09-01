/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oCL_module.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/30 13:58:09 by cledant           #+#    #+#             */
/*   Updated: 2017/09/01 16:45:20 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "oCL_module.hpp"

oCL_module::oCL_module(void)
{
}

oCL_module::~oCL_module(void)
{
}

oCL_module::oCL_module(oCL_module const &src)
{
	static_cast<void>(src);
}

oCL_module		&oCL_module::operator=(oCL_module const &rhs)
{
	static_cast<void>(rhs);
	return (*this);
}

void			oCL_module::oCL_check_error(cl_int const err, cl_int const ref)
{
	if (err != ref)
		throw oCL_module::oCLFailException();
}

void			oCL_module::oCL_create_cl_vbo(GLuint gl_vbo,
					cl::Context const &context, cl::BufferGL &new_buff)
{
	cl_int	err;

	new_buff = cl::BufferGL(context, CL_MEM_WRITE_ONLY, gl_vbo, &err);
	oCL_module::oCL_check_error(err, CL_SUCCESS);
}

void			oCL_module::oCL_init(void)
{
	this->oCL_get_platform_list();
	if (this->oCL_select_platform_from_name("NVIDIA") == false &&
		this->oCL_select_platform_from_name("AMD") == false &&
		this->oCL_select_platform_from_name("Apple") == false)
	{
		std::cout << "No Platform recognized" << std::endl;
		throw oCL_module::oCLFailException();
	}
	this->oCL_get_device_list(CL_DEVICE_TYPE_GPU);
	this->oCL_select_first_oGL_sharing_device();
	std::cout << "OpenCL device : " << this->_cl_device.getInfo<CL_DEVICE_NAME>()
		<< std::endl;
	this->oCL_create_context();
	this->oCL_create_command_queue();
}

cl::Context const		&oCL_module::getContext(void) const
{
	return (this->_cl_context);
}
/*
void			oCL_module::run_kernel(cl::BufferGL &cl_vbo,
					std::string const &name)
{
	cl_int		err;

	glFinish();
	err = this->_cl_cc.enqueueAcquireGLObject();
	
	this->_cl_cc.finish();
}
*/
void			oCL_module::oCL_get_platform_list(void)
{
	cl_int		err;

	err = cl::Platform::get(&(this->_cl_platform_list));
	oCL_module::oCL_check_error(err, CL_SUCCESS);
	if (this->_cl_platform_list.size() == 0)
	{
		std::cout << "No Platform" << std::endl;
		throw oCL_module::oCLFailException();
	}
}

void			oCL_module::oCL_get_device_list(cl_device_type type)
{
	std::vector<cl::Device>::iterator		it;
	cl_int									err;

	err = this->_cl_platform.getDevices(type, &(this->_cl_device_list));
	oCL_module::oCL_check_error(err, CL_SUCCESS);
	if (this->_cl_device_list.size() == 0)
	{
		std::cout << "No Device" << std::endl;
		throw oCL_module::oCLFailException();
	}
}

bool			oCL_module::oCL_select_platform_from_name(std::string const &name)
{
	std::vector<cl::Platform>::iterator		it;
	std::string								value;

	for (it = this->_cl_platform_list.begin();
		it != this->_cl_platform_list.end(); ++it)
	{
		if (it->getInfo(CL_PLATFORM_NAME, &value) == CL_SUCCESS)
		{
			if (value.find(name) == 0)
			{
				this->_cl_platform = *it;
				return (true);
			}
		}
	}
	return (false);
}

void			oCL_module::oCL_select_first_oGL_sharing_device(void)
{
	std::vector<cl::Device>::iterator		it;
	std::string								value;

	for (it = this->_cl_device_list.begin(); it != this->_cl_device_list.end(); ++it)
	{
		if (it->getInfo(CL_DEVICE_EXTENSIONS, &value) == CL_SUCCESS &&
			(value.find("cl_khr_gl_sharing") != std::string::npos |
			value.find("cl_APPLE_gl_sharing") != std::string::npos))
		{
			this->_cl_device = *it;
			return ;
		}
	}
	std::cout << "No Device with OpenGL sharing capability" << std::endl;
	throw oCL_module::oCLFailException();
}

void			oCL_module::oCL_create_context(void)
{
	cl_int				err;
	auto	oCL_error_callback = [](const char *err_info, const void *priv_info_size,
				size_t cb, void *user_data)
	{
		static_cast<void>(priv_info_size);
		static_cast<void>(cb);
		static_cast<void>(user_data);
		std::cout << "Context error :" << err_info << std::endl;
	};
#ifdef __APPLE__
	CGLContextObj		kCGLContext = CGLGetCurrentContext();
	CGLShareGroupObj	kCGLShareGroup = CGLGetShareGroup(kCGLContext);	
	cl_context_properties prop[] =
	{
	  CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
	  (cl_context_properties) kCGLShareGroup,
	  0
	};
#endif

	this->_cl_context = cl::Context({this->_cl_device}, prop, oCL_error_callback,
		NULL, &err);
	oCL_module::oCL_check_error(err, CL_SUCCESS);
}

void			oCL_module::oCL_create_command_queue(void)
{
	cl_int		err;

	this->_cl_cc = cl::CommandQueue(this->_cl_context, this->_cl_device, 0,
		&err);
	oCL_module::oCL_check_error(err, CL_SUCCESS);
}

void			oCL_module::oCL_add_code(std::string const &file)
{
	std::string		kernel;

	oCL_module::read_file(file, kernel);
	this->_cl_sources.push_back({kernel.c_str(), kernel.length()});
}

void			oCL_module::oCL_compile_program(void)
{
	cl_int		err;

	this->_cl_program = cl::Program(this->_cl_context, this->_cl_sources, &err);
	oCL_module::oCL_check_error(err, CL_SUCCESS);
	if ((err = this->_cl_program.build({this->_cl_device})) != CL_SUCCESS)
	{
		std::cout << "OpenCL : Error Building : " <<
			this->_cl_program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(this->_cl_device) <<
			std::endl;
		throw oCL_module::oCLFailException();
	}
}

void			oCL_module::oCL_create_kernel(std::string const &name)
{
	cl_int		err;
	cl::Kernel	kernel;

	kernel = cl::Kernel(this->_cl_program, name.c_str(), &err);
	oCL_module::oCL_check_error(err, CL_SUCCESS);
	this->_cl_kernel_list.push_back(kernel);
}

void			oCL_module::read_file(std::string const &path, std::string &content)
{
	std::fstream	fs;

	fs.open(path, std::fstream::in);
	content.assign((std::istreambuf_iterator<char>(fs)),
		std::istreambuf_iterator<char>());
	fs.close();
}

oCL_module::oCLFailException::oCLFailException(void)
{
	this->_msg = "OpenCL : Something failed !";
}

oCL_module::oCLFailException::~oCLFailException(void) throw()
{
}
