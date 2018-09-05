NAME	= audiowrapper.so

SHELL	= /bin/bash
CC		= clang++
FLG		= -dynamiclib -flat_namespace #-Wall -Werror -Wextra TODO: uncomment

ATTR	= -std=c++11
LBS		= -I ~/.brew/include -L ~/.brew/lib -lSDL2_mixer

$(NAME) :
		$(CC) $(FLG) $(LBS) $(ATTR) audiowrapper.cpp -o $(NAME)

all		: $(NAME)

#TODO:
$(SDL2):
	    #brew install sdl2

clean	: fclean

fclean	:
		@rm -f $(NAME)

re		: fclean all
