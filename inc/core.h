#ifndef __CORE_H
# define __CORE_H

//#include "minilibxwrapper.h"
#include <array>
#include <vector>
#include <string>//TODO: delete this include
#include <sstream>//TODO: delete this include
#include "defines.h"
#include "guidisplay.h"

class AudioWrapper;

class GameCore
{
    enum class Type {
        Snake,
        Obstacle,
        Target
    };

    //////////////////////////////////////////////////////////////////////////////////////////////
    // x, y of left top corner                                                                  //
    //////////////////////////////////////////////////////////////////////////////////////////////
    struct Block
    {
        int     x;
        int     y;
        bool    isVisible;
        Type    type;
        std::uint8_t    *pxls;

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
    int     gameMode;
    int     direction_1;
    int     direction_2;
    int     lastDirection_1;
    int     lastDirection_2;
    int     horizontBlocksNum;
    int     verticalBlocksNum;
    int     m_width;
    int     m_height;
    int     scoreBlockWidth;
    int     scoreBlockHeight;
    int     scoreCount;
    int     currentLib;
    void    *lib_discr;

    Block           *target;
    Block           *bonusTarget;
    GUIDisplay      *disp;
    AudioWrapper    *sound;

    std::uint8_t *snake_body_pixels_map;
    std::uint8_t *snake_h_north_pixels_map;
    std::uint8_t *snake_h_south_pixels_map;
    std::uint8_t *snake_h_west_pixels_map;
    std::uint8_t *snake_h_east_pixels_map;
    std::uint8_t *snake_2_body_pixels_map;
    std::uint8_t *snake_2_h_north_pixels_map;
    std::uint8_t *snake_2_h_south_pixels_map;
    std::uint8_t *snake_2_h_west_pixels_map;
    std::uint8_t *snake_2_h_east_pixels_map;

    std::uint8_t *scorePixelMap;

    std::array<std::uint8_t*, 10> numbersPixelMaps;
    std::array<std::uint8_t*, 5> obstaclePixelMaps;
    std::array<std::uint8_t*, 6> targetPixelMaps;
    std::vector<Block*> obstacles;
    std::vector<Block*> snake_1;
    std::vector<Block*> snake_2;

    //by default backgraund color
    void    setPixelToPixelArray(int sceneX, int sceneY, std::uint8_t *pixels,
                                 int rowLength, std::uint32_t color = 0x186a64ff);
    void    fillBackground(std::uint8_t *pixels, int xFrom, int xTo,
                           int yFrom, int yTo, uint32_t color = 0x186a64ff);
    // void fillBlocks();
    void    insertBlockToScene(int sceneX, int sceneY, int blockWidth,
                               int blockHeight, std::uint8_t *blockPxls, std::uint8_t *scene);
    void    insertElements(std::uint8_t *pixels);
    void    insertScore(std::uint8_t *pixels);
    void    insertScoreCount(std::uint8_t *pixels);
    void    increaseSnake(int nx, int ny, int snakeNumber);
    void    updateSnake(int nx, int ny, std::vector<Block*> snake, int snakeNumber);
    void    updateTarget(Block *target);

    std::uint8_t    *getImage(std::uint8_t *pixels);
    std::uint8_t    *getHeadPixels(int snakeNumber);

    bool    checkTarget(int x, int y, Block* target);
    bool    checkObstacles(int x, int y, std::vector<Block*> snake);

    void    *loadLib(std::string libname);

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Debug functions                                                                          //
    //////////////////////////////////////////////////////////////////////////////////////////////
    int     pixToInt(int x, int y, int rowWidth, std::uint8_t *pixels);
    void    getLib(int libNumber);
    void    getDirection(std::function<void(GUIDisplay*)> release_wrapper);
    void    checkDirection();
    void    initElements();

public:
    //TODO: canonical form!!!!!!!!!
    GameCore(int w, int h, int mode);
    ~GameCore();

    void    run();

};

#endif
