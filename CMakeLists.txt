cmake_minimum_required(VERSION 3.7)
project(Particle_system)

find_package(OpenCL REQUIRED)

#Setup Glad
set(GLAD_API "gl=4.1")
add_subdirectory(${CMAKE_SOURCE_DIR}/glad ${CMAKE_BINARY_DIR}/glad/build)

#Setup GLFW
option(GLFW_BUILD_EXAMPLES "" OFF)
option(GLFW_BUILD_TESTS "" OFF)
option(GLFW_INSTALL "" OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/glfw ${CMAKE_BINARY_DIR}/glfw/build)

#Setup lib compilation
add_executable(particle_system
		srcs/Cubemap.cpp
		srcs/Simple_box.cpp
		srcs/Input.cpp
		srcs/Texture.cpp
		srcs/GeneralException.cpp
		srcs/Camera.cpp
		srcs/IInteractive.cpp
		srcs/Simple_cloud.cpp
		srcs/IEntity.cpp
		srcs/oGL_module.cpp
		srcs/oCL_module.cpp
		srcs/main.cpp
		srcs/GLFW_Window.cpp
		srcs/World.cpp
		srcs/Glfw_manager.cpp
		srcs/Shader.cpp)
add_dependencies(particle_system glad glfw)
target_include_directories(particle_system PRIVATE
		${CMAKE_SOURCE_DIR}/includes
		${CMAKE_SOURCE_DIR}/glm
		${CMAKE_SOURCE_DIR}/stb
		${CMAKE_BINARY_DIR}/glad/build/include
		${CMAKE_SOURCE_DIR}/glfw/include/GLFW)
target_link_libraries(particle_system PRIVATE glad glfw OpenCL::OpenCL GL)
target_compile_options(particle_system PRIVATE -Wall -Wextra -std=c++14)
file(COPY kernels DESTINATION ${CMAKE_BINARY_DIR})
file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY textures DESTINATION ${CMAKE_BINARY_DIR})