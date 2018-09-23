NAME	= imgloader.so

SHELL	= /bin/bash
CC		= clang++
FLG		= -dynamiclib -flat_namespace -Wall -Werror -Wextra

ATTR	= -std=c++11
LBS		= -I ../inc -I ~/.brew/include -L ~/.brew/lib -lsfml-graphics \
			-Wl,-rpath, ~/.brew/lib

$(NAME) :
	$(CC) $(FLG) $(LBS) $(ATTR) imgloaderimpl.cpp -o $(NAME)

all		: $(NAME)

clean	: fclean

fclean	:
	@rm -f $(NAME)

re		: fclean all
