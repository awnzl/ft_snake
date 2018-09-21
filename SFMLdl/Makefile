NAME	= sfmlwrapper.so

SHELL	= /bin/bash
CC		= clang++
FLG		= -dynamiclib -flat_namespace -Wall -Werror -Wextra

ATTR	= -std=c++11
LBS		= -I ../inc -I ~/.brew/include -L ~/.brew/lib -lsfml-system -lsfml-window -lsfml-graphics \
			-Wl,-rpath, ~/.brew/lib -framework OpenGL -framework AppKit

$(NAME) :
	$(CC) $(FLG) $(LBS) $(ATTR) sfmlwrapper.cpp -o $(NAME)

all		: $(NAME)

clean	: fclean

fclean	:
	@rm -f $(NAME)

re		: fclean all
