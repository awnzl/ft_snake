#ifndef __CORE_H
# define __CORE_H

//#include "minilibxwrapper.h"
#include "includes.h"
#include <array>
#include <string>//TODO: delete this include
#include <sstream>//TODO: delete this include

class GameCore
{
    enum class Type {
        Snake,
        Obstacle,
        Target
    };

    //////////////////////////////////////////////////////////////////////////////////////////////
    // x, y of left top corner																	//
    //////////////////////////////////////////////////////////////////////////////////////////////
    struct Block
    {
        int		x;
        int		y;
        bool	isVisible;
        Type    type;
        std::uint8_t	*pxls;

        Block(int x, int y, bool visible, Type type, std::uint8_t *px)
            :
            x(x),
            y(y),
            isVisible(visible),
            type(type),
            pxls(px)
        {
        }

        //TODO: canonical form?

        //TODO: delete this func
        std::string toString()
        {
            std::stringstream ss;
            ss << x << " | " << y << " | " << isVisible << std::endl;
            return ss.str();
        }
    };

    //////////////////////////////////////////////////////////////////////////////////////////////
    //keeps the last moving direction                                                           //
    //////////////////////////////////////////////////////////////////////////////////////////////
    int		direction;

    std::uint8_t target_pixels_map[BLOCK_PIXELS_SIZE];
    std::uint8_t obstacle_pixels_map[BLOCK_PIXELS_SIZE];
    // std::uint8_t snake_pixels_map[BLOCK_PIXELS_SIZE];
    std::uint8_t *snake_pixels_map;

    std::array<Block*, OBSTACLES_QUANTITY> obstacles;//TODO: tmp
    std::array<Block*, TARGETS_QUANTITY> targets;//TODO: tmp
    std::vector <Block*> snake;//TODO: tmp

    void    fillPixelsToPixelsMap(std::uint8_t *px, std::uint32_t color);
    // void	fillBlocks();
    void	insertBlockToScene(int bx, int by, std::uint8_t *block, std::uint8_t *scene);
    //by default backgraund color
    void	setPixelToPixelArray(int x, int y, std::uint8_t *pixels,
                                 int rowLength, std::uint32_t color = 0x186a64ff);
    void	fillBackground(std::uint8_t *image);
    void	insertElements(std::uint8_t *pixels);
    void	initElements();
    void    updateSnake(int nx, int ny);
    void    increaseSnake(int nx, int ny);
    std::uint8_t	*getImage(std::uint8_t *pixels);

    template <std::size_t ARRSIZE>
    bool    checkTargets(int x, int y, std::array<Block*, ARRSIZE> &targets);

    template <std::size_t ARRSIZE>
    bool  checkObstacles(int x, int y, std::array<Block*, ARRSIZE> &);

    template<std::size_t ALLSIZE, std::size_t OBJSIZE, std::size_t TARGETSSIZE>
    void	gameLoop(GUIDisplay &disp,
                     std::array<Block*, ALLSIZE> &allElements,
                     std::array<Block*, OBJSIZE> &obstacles,
                     std::array<Block*, TARGETSSIZE> &targets);

    void *load_lib(std::string libname);

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Debug functions																			//
    //////////////////////////////////////////////////////////////////////////////////////////////
    int pixToInt(int x, int y, std::uint8_t *pixels);

    AudioWrapper sound;

public:
    //TODO: canonical form!!!!!!!!!
    GameCore();
    ~GameCore();
    void run();
};

#endif
