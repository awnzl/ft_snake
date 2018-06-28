#include <iostream>
#include <functional>
#include <unistd.h>
#include "core.h"
#include "minilibxwrapper.h"

GameCore::GameCore()
{
    //TODO: temporary block start:
    fillPixelsToPixelsMap(target_pixels_map, 0x00ff00);
    fillPixelsToPixelsMap(obstacle_pixels_map, 0x0000ff);
    // temporary block end
}

GameCore::~GameCore()
{
    //TODO: free blocks arrays content 
    for (int i = 0; i < obstacles.size(); i++)
        delete obstacles[i];
    for (int i = 0; i < targets.size(); i++)
        delete targets[i];
}

//uses for fill pixel arrays for blocks
void    GameCore::fillPixelsToPixelsMap(uint8_t *px, int color)
{
    for (int y = 0; y < BLOCK_SIZE; y++)
        for (int x = 0; x < BLOCK_SIZE; x++)
            setPixel(x, y, px, BLOCK_SIZE, color);
}

//first init and need function for replace on each cycle!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// void    GameCore::fillBlocks() {
//     for (int i = 0, x = 0, y = 0; i < BLOCKS_QUANTITY; i++)
//     {
//         blocks[i] = new Block(x, y, true, background_pxls);
//         x++;
//         if (!(x % (WIDTH_HEIGTH / BLOCK_SIZE)))
//         {
//             std::cout << "fillBlocks Debug: x = " << x;
//             x = 0;
//             y++;
//             // std::cout << "; y = " << y << std::endl;//TODO: remove
//         }
//         // std::cout << "debug: x = " << x << "; y = " << y << "; i = " << i << std::endl;
//     }
//     // exit(1);
// }

void    GameCore::setPixel(int x, int y, uint8_t *pixels, int rowLength, int color)
{
    int idx = (y * rowLength + x) * 4;
    // pixels[idx + 3] = color >> 24; //a
    // pixels[idx + 2] = color >> 16; //r
    // pixels[idx + 1] = color >> 8;  //g
    // pixels[idx + 0] = color;       //b
    //next works for sfml (becouse alpha [3] and red [2])
    pixels[idx + 0] = color >> 24; //b
    pixels[idx + 1] = color >> 16; //g
    pixels[idx + 2] = color >> 8;  //r
    pixels[idx + 3] = color;       //a
}

void    GameCore::fillBackground(uint8_t *pixels)
{
    for (int y = 0; y < WIDTH_HEIGTH; y++)
        for (int x = 0; x < WIDTH_HEIGTH; x++)
            setPixel(x, y, pixels, WIDTH_HEIGTH);
}

void    GameCore::insertSnake(uint8_t *pixels)
{
    for (int y = snake.y - BLOCK_SIZE / 2; y < snake.y + BLOCK_SIZE / 2; y++)
        for (int x = snake.x - BLOCK_SIZE / 2; x < snake.x + BLOCK_SIZE / 2; x++)
            setPixel(x, y, pixels, WIDTH_HEIGTH, 0xff0000);
}

//		1
//	2	0	3
//		4
//a w s d
uint8_t    *GameCore::getImage(int event, uint8_t *pixels)
{
    switch (event)
    {
        case (1):
            snake.y -= (snake.y - 10 - BLOCK_SIZE / 2  <= 0) ? 0 : 10;
            break;
        case (2):
            snake.x -= (snake.x - 10 - BLOCK_SIZE / 2 <= 0) ? 0 : 10;
            break;
        case (3):
            snake.x += (snake.x + 10 + BLOCK_SIZE / 2 >= WIDTH_HEIGTH) ? 0 : 10;
            break;
        case (4):
            snake.y += (snake.y + 10 + BLOCK_SIZE / 2 >= WIDTH_HEIGTH) ? 0 : 10;
            break;
    }
    //TODO: add frame to window
    fillBackground(pixels);
    insertElements(pixels);
    insertSnake(pixels);
    // insertObstacles(pixels);
    // insertTargets(pixels);
    return pixels;
}

void    GameCore::initElements()
{
    //init obstacles
    //init targets
    obstacles[0] = new Block(2*32, 3*32, true, obstacle_pixels_map);
    obstacles[1] = new Block(12*32, 5*32, true, obstacle_pixels_map);
    obstacles[2] = new Block(6*32, 13*32, true, obstacle_pixels_map);
    obstacles[3] = new Block(16*32, 3*32, true, obstacle_pixels_map);

    targets[0] = new Block(3*32, 4*32, true, target_pixels_map);
    targets[1] = new Block(7*32, 8*32, true, target_pixels_map);
    targets[2] = new Block(11*32, 12*32, true, target_pixels_map);
    targets[3] = new Block(23*32, 18*32, true, target_pixels_map);
}

//think about this implementation... first color gather into integer, than disperse
void    GameCore::insertBlockToScene(int bx, int by, uint8_t *block, uint8_t *scene)
{
    int idx = 0;
    for (int y = 0; y < BLOCK_SIZE; y++)
        for (int x = 0; x < BLOCK_SIZE; x++)
        {
            setPixel(x + bx, y + by, scene, WIDTH_HEIGTH, ((block[idx] << 24) +
                                   (block[idx + 1] << 16) +
                                   (block[idx + 2] << 8) +
                                   (block[idx + 3])));
            idx += 4;
        }
}

void    GameCore::insertElements(uint8_t *pixels)
{
    //insert obstacles, targets, snake
    for (int i = 0; i < obstacles.size(); i++)
        if (obstacles[i]->isVisible)
            insertBlockToScene(obstacles[i]->x,
                               obstacles[i]->y,
                               obstacles[i]->pxls,
                               pixels);
    for (int i = 0; i < targets.size(); i++)
        if (targets[i]->isVisible)
            insertBlockToScene(targets[i]->x,
                               targets[i]->y,
                               targets[i]->pxls,
                               pixels);
    int idx = 4;
}

// template<std::size_t ALLSIZE, std::size_t OBJSIZE, std::size_t TARGETSSIZE>
// void    GameCore::gameLoop(GUIDisplay &disp,
//                            std::array<Block*, ALLSIZE> &allElements,
//                            std::array<Block*, OBJSIZE> &obstacles
//                            std::array<Block*, TARGETSSIZE> &targets)
// {
//     int isRun = 0;
//     uint8_t pixels[ARRAY_SIZE];

//     while (isRun > -1) {
//         isRun = disp.run(getImage(isRun, pixels));
//         std::cout << "debug: libreturn: " << isRun << std::endl;
//     }
// }

void	GameCore::run()
{
    GUIDisplay disp;

    // std::array<Block*, BLOCKS_QUANTITY> allElenents;
	// std::array<Block*, OBSTACLES_QUANTITY> obstacles;
	// std::array<Block*, TARGETS_QUANTITY> targets;

    initElements();

    int isRun = 0;
    uint8_t pixels[ARRAY_SIZE];

    while (isRun > -1) {
        isRun = disp.run(getImage(isRun, pixels));
        // std::cout << "debug: libreturn: " << isRun << std::endl;
    }

    // gameLoop(disp, allElenents, obstacles, targets);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Debug functions																			    //
//////////////////////////////////////////////////////////////////////////////////////////////////

int GameCore::pixToInt(int x, int y, uint8_t *pixels)
{
    int idx = (y * WIDTH_HEIGTH + x) * 4;
    // return ((pixels[idx + 3] << 24) +
    //         (pixels[idx + 2] << 16) +
    //         (pixels[idx + 1] << 8) + (pixels[idx]));
    //for sfml:
    return ((pixels[idx] << 24) +
            (pixels[idx + 1] << 16) +
            (pixels[idx + 2] << 8) +
            (pixels[idx + 3]));
}