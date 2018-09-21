NAME	= glfwwrapper.so

SHELL	= /bin/bash
CC		= clang++
FLG		= -dynamiclib -flat_namespace -Wall -Werror -Wextra

ATTR	= -std=c++11
LBS		= -I ../inc -I ~/.brew/include -L ~/.brew/lib -lGLFW -framework OpenGL -framework AppKit

$(NAME) :
	$(CC) $(FLG) $(LBS) $(ATTR) glfwwrapper.cpp -o $(NAME)

all		: $(NAME)

clean	: fclean

fclean	:
	@rm -f $(NAME)

re		: fclean all
