NAME	= nibbler
DLLIBS	=

SHELL	= /bin/bash
CC		= clang++
FLG		= -Wall -Werror -Wextra

SRCDIR	= ./src
INCDIR	= ./inc
OBJDIR	= ./obj

SRC		= main.cpp core.cpp timer.cpp

ATTR	= -std=c++11
LBS		= -I ~/.brew/include
WRP		= -I AudioWrapper -I IMGLoader


OBJ		= $(addprefix $(OBJDIR)/,$(SRC:.cpp=.o))

$(NAME) : exp obj $(OBJ)
	$(CC) $(FLG) $(OBJ) $(ATTR) $(LBS) -o $(NAME)


all		: $(NAME)

exp	:
	@. ./define-my-variables.sh
	@make -C SFMLdl/
	@make -C GLFWdl/
	@make -C SDL2dl/
	@make -C IMGLoader/
	@make -C AudioWrapper/


obj		:
	@mkdir -p $(OBJDIR)


$(OBJDIR)/%.o:$(SRCDIR)/%.cpp
	$(CC) $(FLG) $(ATTR) -I $(INCDIR) $(WRP) $(LBS) -o $@ -c $<

clean	:
	rm -rf $(OBJDIR)

fclean	: clean
	rm -f $(NAME)
	make -C GLFWdl/ clean
	make -C SFMLdl/ clean
	make -C SDL2dl/ clean
	make -C IMGLoader/ clean
	make -C AudioWrapper/ clean

re		: fclean all
