#include "core.h"
#include <iostream>
#include <regex>

void printUsage()
{
    std::cout << "\n"
              << "         >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>     Nibbler    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n"
              << "\n"
              << "         Nibbler is a classic snake game. It is necessary to collect as much food as possible\n"
              << "         avoiding collision with obstacles and own tail. Every time a snake eats a piece of\n"
              << "         food, it becomes longer.\n"
              << "\n"
              << "         To start the game, you must specify the width and the height of the playing field.\n"
              << "         For example: ./nibbler 40 40\n"
              << "\n"
              << "         Minimum field sizes: 30x30 \n"
              << "         Maximum field sizes: 99х99\n"
              << "         Snake control, keys: UP, DOWN, RIGHT, LEFT\n"
              << "         To exit the game:    ESC\n"
              << "\n";
    exit(1);
}

int main(int ac, char *av[])
{
    srand(time(NULL));
    int width, height;
    std::cmatch result;
    std::regex regular("[0-9]+");

    if (ac != 3)
        printUsage();
    std::string s1 = av[1], s2 = av[2];
    try
    {
        std::string exceptionMessage("\nEXEPTION: Incorrect value of field sizes.\n"
                                      "Minimum field sizes: 30x30 \n"
                                      "Maximum field sizes: 99х99\n");

        if (std::regex_match(s1.c_str(), result, regular) &&
            std::regex_match(s2.c_str(), result, regular))
        {
            width = std::stoi(av[1]);
            height = std::stoi(av[2]);
            if(width < 30 || height < 30 || width > 99 || height > 99)
                throw std::invalid_argument(exceptionMessage);
            GameCore *gc = new GameCore(width, height);
            // timer.setTimeScale(0.2f);//TODO: replace by value of mandatory's requiroment
            gc->initElements();
            gc->startGame(10);
            delete gc;
        }
        else
            throw std::invalid_argument(exceptionMessage);
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
        exit(1);
    }
    return (0);
}