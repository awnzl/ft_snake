#include <iostream>
#include <functional>
#include <unistd.h>
#include "core.h"
#include "minilibxwrapper.h"

GameCore::GameCore() : direction(3)
{
    //TODO: temporary block start:
    fillPixelsToPixelsMap(target_pixels_map, 0x00ff00);
    fillPixelsToPixelsMap(obstacle_pixels_map, 0x0000ff);
    fillPixelsToPixelsMap(snake_pixels_map, 0xff0000);
    // temporary block end
}

GameCore::~GameCore()
{
    //TODO: free blocks arrays content
    for (auto e: obstacles)
        delete e;
    for (auto e: targets)
        delete e;
    for (auto e: snake)
        delete e;
}

//uses for fill pixel arrays for blocks
void    GameCore::fillPixelsToPixelsMap(uint8_t *px, int color)
{
    for (int y = 0; y < BLOCK_SIZE; y++)
        for (int x = 0; x < BLOCK_SIZE; x++)
            setPixelToPixelArray(x, y, px, BLOCK_SIZE, color);
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
//             // std::cout << "; y = " << y << std::endl;
//         }
//         // std::cout << "debug: x = " << x << "; y = " << y << "; i = " << i << std::endl;
//     }
//     // exit(1);
// }

void    GameCore::setPixelToPixelArray(int x, int y, uint8_t *pixels, int rowLength, int color)
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
            setPixelToPixelArray(x, y, pixels, WIDTH_HEIGTH);
}

void    GameCore::insertBlockToScene(int bx, int by, uint8_t *block, uint8_t *scene)
{
    for (int y = 0; y < BLOCK_SIZE; y++)
    {
        int idx = ((by + y) * WIDTH_HEIGTH + bx) * 4;
        for (int i = 0; i < BLOCK_SIZE * 4; i++)
            scene[idx++] = block[i];
    }
}

void    GameCore::insertElements(uint8_t *pixels)
{
    //insert obstacles, targets, snake
    for (auto e: obstacles)
        if (e->isVisible)
            insertBlockToScene(e->x, e->y, e->pxls, pixels);

    for (auto e: targets)
        if (e->isVisible)
            insertBlockToScene(e->x, e->y, e->pxls, pixels);

    for (auto e: snake)
        insertBlockToScene(e->x, e->y, e->pxls, pixels);
}

void    GameCore::initElements()
{
    //init obstacles
    //init targets
    obstacles[0] = new Block(3*32, 3*32, true, Type::Obstacle, obstacle_pixels_map);
    obstacles[1] = new Block(1*32, 1*32, true, Type::Obstacle, obstacle_pixels_map);
    obstacles[2] = new Block(6*32, 13*32, true, Type::Obstacle, obstacle_pixels_map);
    obstacles[3] = new Block(16*32, 3*32, true, Type::Obstacle, obstacle_pixels_map);

    targets[0] = new Block(3*32, 4*32, true, Type::Target, target_pixels_map);
    targets[1] = new Block(7*32, 8*32, true, Type::Target, target_pixels_map);
    targets[2] = new Block(11*32, 12*32, true, Type::Target, target_pixels_map);
    targets[3] = new Block(23*32, 18*32, true, Type::Target, target_pixels_map);

    snake.push_back(new Block(512, 448, true, Type::Snake, snake_pixels_map));
    snake.push_back(new Block(480, 448, true, Type::Snake, snake_pixels_map));
    snake.push_back(new Block(448, 448, true, Type::Snake, snake_pixels_map));
    snake.push_back(new Block(416, 448, true, Type::Snake, snake_pixels_map));
}

bool    GameCore::checkObstacles(int x, int y)
{
    for (auto each: obstacles)
        if (each->x == x && each->y == y)
            return true;
    return false;
}

void    GameCore::updateSnake(int nx, int ny)
{
    int previousElementX = snake[0]->x;
    int previousElementY = snake[0]->y;
    int tmpX, tmpY;

    snake[0]->x = nx;
    snake[0]->y = ny;

    for (int idx = 1; idx < snake.size(); idx++)
    {
        tmpX = snake[idx]->x;
        tmpY = snake[idx]->y;
        snake[idx]->x = previousElementX;
        snake[idx]->y = previousElementY;
        previousElementX = tmpX;
        previousElementY = tmpY;
    }
}

//		1
//	2	0	3
//		4
//a w s d
uint8_t    *GameCore::getImage(uint8_t *pixels)
{
    int nextX = snake[0]->x;
    int nextY = snake[0]->y;

    switch (direction)
    {
        case (1):
            nextY -= (snake[0]->y - BLOCK_SIZE < 0) ? 0 : BLOCK_SIZE;
            break;
        case (2):
            nextX -= (snake[0]->x - BLOCK_SIZE < 0) ? 0 : BLOCK_SIZE;
            break;
        case (3):
            nextX += (snake[0]->x + BLOCK_SIZE >= WIDTH_HEIGTH) ? 0 : BLOCK_SIZE;
            break;
        case (4):
            nextY += (snake[0]->y + BLOCK_SIZE >= WIDTH_HEIGTH) ? 0 : BLOCK_SIZE;
            break;
    }

    if (!checkObstacles(nextX, nextY))
    {
        updateSnake(nextX, nextY);
        // snake[0]->x = nextX;
        // snake[0]->y = nextY;
    }
    // std::cout << "snX " << snake[0]->x << " snY " << snake[0]->y << " ";
    //TODO: add frame to window
    fillBackground(pixels);
    insertElements(pixels);
    return pixels;
}

// template<std::size_t OBJSIZE, std::size_t TARGETSSIZE>
// void    GameCore::gameLoop(GUIDisplay &disp,
//                            std::array<Block*, OBJSIZE> &obstacles
//                            std::array<Block*, TARGETSSIZE> &targets)
// {
//     int event = 0;
//     uint8_t pixels[ARRAY_SIZE];

//     while (event > -1) {
//         event = disp.run(getImage(event, pixels));
//         std::cout << "debug: libreturn: " << event << std::endl;
//     }
// }

void	GameCore::run()
{
    GUIDisplay disp;

    Timer timer;

	// std::array<Block*, OBSTACLES_QUANTITY> obstacles;
	// std::array<Block*, TARGETS_QUANTITY> targets;

    initElements();

    uint8_t pixels[ARRAY_SIZE];

    while (direction)
    {
        timer.tick();
        if (timer.deltaTime() >= timer.getTimeScale())
        {
            timer.reset();
            disp.render(getImage(pixels));
        }
        direction = disp.getEvent();
        // std::cout << "debug: libreturn: " << direction << std::endl;
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