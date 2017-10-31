cmake_minimum_required(VERSION 3.8)
project(particle_system)

#Setup Glad
set(GLAD_API "gl=4.1")
add_subdirectory(glad ${CMAKE_BINARY_DIR}/glad/build)

#Setup GLFW
option(GLFW_BUILD_EXAMPLES "" OFF)
option(GLFW_BUILD_TESTS "" OFF)
option(GLFW_INSTALL "" OFF)
add_subdirectory(glfw ${CMAKE_BINARY_DIR}/glfw/build)

#Setup particle_system
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Werror -Wextra -O2")
include_directories(includes glm stb glad/build/includes glfw/include/GLFW)
link_directories(${CMAKE_BINARY_DIR}/glad/build ${CMAKE_BINARY_DIR}/glfw/build/src)
set(SOURCE_FILES
        srcs/Camera.cpp
        srcs/Cubemap.cpp
        srcs/GeneralException.cpp
        srcs/Glfw_manager.cpp
        srcs/IEntity.cpp
        srcs/IInteractive.cpp
        srcs/Input.cpp
        srcs/main.cpp
        srcs/oCL_module.cpp
        srcs/oGL_module.cpp
        srcs/Shader.cpp
        srcs/Simple_box.cpp
        srcs/Simple_cloud.cpp
        srcs/Texture.cpp
        srcs/Window.cpp
        srcs/World.cpp)

#OS specific libs
if(APPLE)
    find_library(OPEN_GL OpenGL REQUIRED)
    find_library(COCOA COCOA REQUIRED)
    find_library(IOKIT IOkit REQUIRED)
    find_library(COREVIDEO CoreVideo REQUIRED)
    find_library(OPEN_CL OpenCL REQUIRED)
    set(EXTRA_LIBS ${OPEN_GL} ${COCOA} ${IOKIT} ${COREVIDEO} ${OPEN_CL})
endif()

add_executable(particle_system ${SOURCE_FILES})
target_link_libraries(particle_system glad glfw ${EXTRA_LIBS})
file(COPY kernels DESTINATION ${CMAKE_BINARY_DIR})
file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY textures DESTINATION ${CMAKE_BINARY_DIR})