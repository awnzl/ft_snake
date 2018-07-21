#ifndef __CORE_H
# define __CORE_H

//#include "minilibxwrapper.h"
#include "defines.h"
#include "sfmlwrapper.h"
#include "timer.h"
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
        uint8_t	*pxls;

        Block(int x, int y, bool visible, Type type, uint8_t *px)
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

    uint8_t target_pixels_map[BLOCK_PIXELS_SIZE];
    uint8_t obstacle_pixels_map[BLOCK_PIXELS_SIZE];
    uint8_t snake_pixels_map[BLOCK_PIXELS_SIZE];

    std::array<Block*, OBSTACLES_QUANTITY> obstacles;//TODO: tmp
    std::array<Block*, TARGETS_QUANTITY> targets;//TODO: tmp
    std::vector <Block*> snake;//TODO: tmp

    void    fillPixelsToPixelsMap(uint8_t *px, int color);
    // void	fillBlocks();
    void	insertBlockToScene(int bx, int by, uint8_t *block, uint8_t *scene);
    //by default backgraund color
    void	setPixelToPixelArray(int x, int y, uint8_t *pixels, int rowLength, int color = 0x30D5C8);
    void	fillBackground(uint8_t *image);
    void	insertElements(uint8_t *pixels);
    void	initElements();
    bool    checkObstacles(int x, int y);
    void    updateSnake(int nx, int ny);
    uint8_t	*getImage(uint8_t *pixels);

    template<std::size_t ALLSIZE, std::size_t OBJSIZE, std::size_t TARGETSSIZE>
    void	gameLoop(GUIDisplay &disp,
                     std::array<Block*, ALLSIZE> &allElements,
                     std::array<Block*, OBJSIZE> &obstacles,
                     std::array<Block*, TARGETSSIZE> &targets);

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Debug functions																			//
    //////////////////////////////////////////////////////////////////////////////////////////////
    int pixToInt(int x, int y, uint8_t *pixels);

public:
    //TODO: canonical form!!!!!!!!!
    GameCore();
    ~GameCore();
    void run();
};

#endif
