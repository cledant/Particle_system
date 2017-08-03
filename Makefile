# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cledant <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/11/26 10:40:13 by cledant           #+#    #+#              #
#    Updated: 2017/08/03 18:59:25 by cledant          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = clang++

CFLAGS = --std=c++14 -Werror -Wall -Wextra -O2

OBJ_DIR_NAME = obj

LIBS = -lglfw3

FRAMEWORK = -framework OpenGL -framework Cocoa -framework IOkit -framework CoreVideo

INCLUDES_GLFW = ./glfw/include/GLFW

GLFW_NAME = libglfw3.a

GLFW_DIR = ./glfw

GLFW_BUILD_DIR = ./glfw/build

GLFW_PATH = $(GLFW_BUILD_DIR)/src

SRCS_NAME = main.cpp Window.cpp Input.cpp Glfw_manager.cpp GeneralException.cpp

INCLUDES = ./includes

SRCS_PATH = ./srcs

OBJ_SRCS = $(SRCS_NAME:%.cpp=$(OBJ_DIR_NAME)/%.o)

NAME = particle_system

all : glfw $(NAME)

glfw : $(GLFW_PATH)/$(GLFW_NAME)

$(GLFW_PATH)/$(GLFW_NAME) :
	cmake -B$(GLFW_BUILD_DIR) -H$(GLFW_DIR)
	make -C $(GLFW_BUILD_DIR)

$(NAME) : $(OBJ_SRCS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) -L$(GLFW_PATH) $(FRAMEWORK)

$(OBJ_DIR_NAME)/%.o : $(SRCS_PATH)/%.cpp
	mkdir -p $(OBJ_DIR_NAME)
	$(CC) -o $@ -c $< $(CFLAGS) -I$(INCLUDES) -I$(INCLUDES_GLFW)

clean :
	rm -rf $(OBJ_DIR_NAME)

fclean : clean
	rm -rf $(NAME)
	rm -rf $(GLFW_BUILD_DIR)

re : fclean all

.PHONY : all clean fclean re glfw
