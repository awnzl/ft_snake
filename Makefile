NAME	= nibbler
SDL2	= libsdl2

SHELL	= /bin/bash
CC		= clang++
FLG		= #-Wall -Werror -Wextra TODO: uncomment

SRCDIR	= ./src
INCDIR	= ./inc
OBJDIR	= ./obj

SRC		= main.cpp core.cpp timer.cpp #pngreader.cpp #minilibxwrapper.cpp

$LEXP1	= export CPLUS_INCLUDE_PATH=/Users/itsuman/.brew/include
$LEXP2	= export LD_LIBRARY_PATH=/User/itsuman/.brew/Cellar/sfml/2.4.2_1/lib1

ATTR	= -std=c++11
LBS		= -I ~/.brew/include -L ~/.brew/lib -lsfml-system -lsfml-window -lsfml-graphics -lsfml-network -lsfml-audio \
			-Wl,-rpath, ~/.brew/lib -lGLFW -lpng -framework OpenGL -framework AppKit


OBJ		= $(addprefix $(OBJDIR)/,$(SRC:.cpp=.o))

$(NAME) : exp obj $(OBJ)
	$(CC) $(FLG) $(OBJ) $(ATTR) $(LBS) -o $(NAME)


all		: $(SDL2) $(NAME)

$(SDL2):
    #brew install sdl2

exp	:
	set -e; \
    . ./define-my-variables.sh

# echo SHELL export CPLUS_INCLUDE_PATH=/Users/itsuman/.brew/include
# echo SHELL export LD_LIBRARY_PATH=/User/itsuman/.brew/Cellar/sfml/2.4.2_1/lib
#нужно понять, как сделать экспорт переменных и выполнить баш код

obj		:
	@mkdir -p $(OBJDIR)


$(OBJDIR)/%.o:$(SRCDIR)/%.cpp
	$(CC) $(FLG) $(ATTR) -I $(INCDIR) -o $@ -c $<

clean	:
	rm -rf $(OBJDIR)

fclean	: clean
	rm -f $(NAME)

re		: fclean all
