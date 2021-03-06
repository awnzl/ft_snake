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
              << "         For example: ./nibbler 20 10\n"
              << "\n"
              << "         Minimum field sizes: 10x10 \n"
              << "         Maximum field sizes: 50х50\n"
              << "         Snake control, keys: UP, DOWN, RIGHT, LEFT\n"
              << "         To exit the game:    ESC\n"
              << "\n";
    exit(1);
}

int main(int ac, char *av[])
{
    srand(time(NULL));
    int width, height, gameMode;
    std::cmatch result;
    std::regex regular("[0-9]+");

    if (ac < 3 || ac > 4)
        printUsage();
    std::string s1 = av[1], s2 = av[2], s3;
    if (ac == 4)
        s3 = av[3];
    else
        s3 = "1";
    try
    {
        std::string exceptionMessage("\nEXEPTION: Incorrect value of field sizes.\n"
                                      "Minimum field sizes: 10x10\n"
                                      "Maximum field sizes: 50х50\n");
        if (std::regex_match(s1.c_str(), result, regular) &&
            std::regex_match(s2.c_str(), result, regular) &&
            std::regex_match(s3.c_str(), result, regular))
        {
            width = std::stoi(av[1]);
            height = std::stoi(av[2]);
            gameMode = std::stoi(s3);
            if(width < 10 || height < 10 || width > 50 || height > 50 || gameMode > 2)
                throw std::invalid_argument(exceptionMessage);
            GameCore *gc = new GameCore(width, height, gameMode);
            gc->run();
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