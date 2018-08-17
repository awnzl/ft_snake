NAME	= sfmlwrapper.so

SHELL	= /bin/bash
CC		= clang++
FLG		= -dynamiclib -flat_namespace #-Wall -Werror -Wextra TODO: uncomment

ATTR	= -std=c++11
LBS		= -I ~/.brew/include -L ~/.brew/lib -lsfml-system -lsfml-window -lsfml-graphics \
			-Wl,-rpath, ~/.brew/lib -framework OpenGL -framework AppKit

$(NAME) :
	$(CC) $(FLG) $(LBS) $(ATTR) sfml2wrapper.cpp -o $(NAME)

all		: $(NAME)

$(SDL2):
    #brew install sdl2

clean	: fclean

fclean	:
	rm -f $(NAME)

re		: fclean all
