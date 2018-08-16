#include "core.h"
#include <iostream>
#include <string>
#include <regex>

void printUsage()
{
	std::cout << "" << std::endl;
	std::cout << "         >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>     Nibbler     <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "         Nibbler is a classic snake game. It is necessary to collect as much food as\n"
				 "         possible avoiding collision with obstacles and own tail. Every time a snake\n"
				 "         eats a piece of food, it becomes longer.\n" 
				 "\n"
				 "         To start the game, you must specify the width and height of the playing field.\n"
				 "         For example: ./nibbler 40 40\n"
				 "\n"
				 "         Minimum field sizes: 30x30 \n"
				 "         Maximum field sizes: 99х99\n"
				 "         Snake control, keys: UP, DOWN, RIGHT, LEFT\n"
				 "         To exit the game:    ESC\n" << std::endl;
	exit(1);
}

void printExeption()
{
	std::cout << "\nEXEPTION: Incorrect value of field sizes.\n" 
				 "Minimum field sizes: 30x30 \n"
				 "Maximum field sizes: 99х99\n" << std::endl;
	exit(1);
}

int main(int ac, char *av[])
{
	int width, height;
	std::cmatch result;
	std::regex regular("[0-9]+");
	
	if(ac != 3)
		printUsage();
	std::string s1 = av[1], s2 = av[2];
	try {
		if(std::regex_match(s1.c_str(), result, regular) && std::regex_match(s2.c_str(), result, regular)) {
				width = std::stoi(av[1]);
				height = std::stoi(av[2]);
				if(width < 30 || height < 30 || width > 99 || height > 99)
					printExeption();
				GameCore *gc = new GameCore;
				gc->run();
				delete gc;
		}
		else printExeption();
	}
	catch (...) {
		printExeption();
	}
	return (0);
}