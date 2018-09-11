NAME	= nibbler
DLLIBS	=

SHELL	= /bin/bash
CC		= clang++
FLG		= #-Wall -Werror -Wextra TODO: uncomment

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

#TODO: need to implement lib checking, exports, install
exp	:
# 	set -e; \
#     . ./define-my-variables.sh
# 	@make -C GLFWdl/
# 	@make -C SFMLdl/
# 	@make -C SDL2dl/
# 	@make -C IMGLoader/
# 	@make -C AudioWrapper/

# echo SHELL export CPLUS_INCLUDE_PATH=/Users/itsuman/.brew/include
# echo SHELL export LD_LIBRARY_PATH=/User/itsuman/.brew/Cellar/sfml/2.4.2_1/lib
#нужно понять, как сделать экспорт переменных и выполнить баш код

obj		:
	@mkdir -p $(OBJDIR)


$(OBJDIR)/%.o:$(SRCDIR)/%.cpp
	$(CC) $(FLG) $(ATTR) -I $(INCDIR) $(WRP) $(LBS) -o $@ -c $<

clean	:
	rm -rf $(OBJDIR)

fclean	: clean
	rm -f $(NAME)

re		: fclean all
