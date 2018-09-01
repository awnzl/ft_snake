#ifndef __CORE_H
# define __CORE_H

//#include "minilibxwrapper.h"
#include <array>
#include <vector>
#include <string>//TODO: delete this include
#include <sstream>//TODO: delete this include
#include "defines.h"
#include "guidisplay.h"
#include "audiowrapper.h"

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
    int     lastDirection;
    int     m_width;
    int     m_height;
    int     currentLib;

    Block           *target;
    Block           *bonusTarget;
    GUIDisplay      *disp;
    AudioWrapper    sound;

    std::uint8_t target_pixels_map[BLOCK_PIXELS_SIZE];
    std::uint8_t obstacle_pixels_map[BLOCK_PIXELS_SIZE];
    // std::uint8_t snake_pixels_map[BLOCK_PIXELS_SIZE];
    std::uint8_t *snake_pixels_map;

    std::vector<Block*> obstacles;
    std::vector<Block*> snake;

    void    fillPixelsToPixelsMap(std::uint8_t *px, std::uint32_t color);
    // void	fillBlocks();
    void	insertBlockToScene(int bx, int by, std::uint8_t *block, std::uint8_t *scene);
    //by default backgraund color
    void	setPixelToPixelArray(int x, int y, std::uint8_t *pixels,
                                 int rowLength, std::uint32_t color = 0x186a64ff);
    void	fillBackground(std::uint8_t *image);
    void	insertElements(std::uint8_t *pixels);
    void    updateSnake(int nx, int ny);
    void    updateTarget(Block *target);
    void    increaseSnake(int nx, int ny);
    
    std::uint8_t	*getImage(std::uint8_t *pixels);

    bool    checkTarget(int x, int y, Block* target);
    bool    checkObstacles(int x, int y);

    // template<std::size_t ALLSIZE, std::size_t OBJSIZE, std::size_t TARGETSSIZE>
    // void	gameLoop(GUIDisplay &disp,
    //                  std::array<Block*, ALLSIZE> &allElements,
    //                  std::array<Block*, OBJSIZE> &obstacles,
    //                  std::array<Block*, TARGETSSIZE> &targets);

    void    *load_lib(std::string libname);

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Debug functions																			//
    //////////////////////////////////////////////////////////////////////////////////////////////
    int     pixToInt(int x, int y, std::uint8_t *pixels);
    void    run(void    *lib_discr);

public:
    //TODO: canonical form!!!!!!!!!
    GameCore(int w, int h);
    ~GameCore();
    
    void	initElements();
    void    startGame(int libNumber);
};

#endif
