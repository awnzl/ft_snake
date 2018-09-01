NAME	= imgloader.so

SHELL	= /bin/bash
CC		= clang++
FLG		= -dynamiclib -flat_namespace #-Wall -Werror -Wextra TODO: uncomment

ATTR	= -std=c++11
LBS		= -I ../inc -I ~/.brew/include -L ~/.brew/lib -lsfml-graphics \
			-Wl,-rpath, ~/.brew/lib -framework OpenGL -framework AppKit

$(NAME) :
	$(CC) $(FLG) $(LBS) $(ATTR) imgloader.cpp -o $(NAME)

all		: $(NAME)

$(SDL2):
    #brew install sdl2

clean	: fclean

fclean	:
	rm -f $(NAME)

re		: fclean all
