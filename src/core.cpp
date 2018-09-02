#include <dlfcn.h>
#include <iostream>
#include <functional>
#include <unistd.h>
#include "core.h"
#include "timer.h"
#include "../IMGLoader/imgloader.h"

GameCore::GameCore(int w, int h) :
    direction(3),
    lastDirection(3),
    m_width(w * BLOCK_SIZE),
    m_height(h * BLOCK_SIZE)
{
    //TODO: temporary block start:
    fillPixelsToPixelsMap(target_pixels_map, 0xf0ff0fFF);
    fillPixelsToPixelsMap(obstacle_pixels_map, 0xFF0000FF);

    void *lib_discr = load_lib("IMGLoader/imgloader.so");
    std::function<IMGLoader*()> create_loader((IMGLoader*(*)())dlsym(lib_discr, "createImgLoader"));
    std::function<void(IMGLoader*)> release_loader((void(*)(IMGLoader*))dlsym(lib_discr, "releaseImgLoader"));
    IMGLoader *imloader = create_loader();
    snake_body_pixels_map = imloader->getPixelMap("assets/body.png");
    snake_h_north_pixels_map = imloader->getPixelMap("assets/head_north.png");
    snake_h_south_pixels_map = imloader->getPixelMap("assets/head_south.png");
    snake_h_west_pixels_map = imloader->getPixelMap("assets/head_west.png");
    snake_h_east_pixels_map = imloader->getPixelMap("assets/head_east.png");

    release_loader(imloader);
}

GameCore::~GameCore()
{
    //TODO: free blocks arrays content
    for (auto e: obstacles)
        delete e;
    for (auto e: snake)
        delete e;
    delete target;
    delete bonusTarget;
}

//uses to fill pixel arrays for blocks
void    GameCore::fillPixelsToPixelsMap(std::uint8_t *px, uint32_t color)
{
    for (int y = 0; y < BLOCK_SIZE; y++)
        for (int x = 0; x < BLOCK_SIZE; x++)
            setPixelToPixelArray(x, y, px, BLOCK_SIZE, color);
}

void    GameCore::setPixelToPixelArray(int x, int y, std::uint8_t *pixels,
                                       int rowLength, uint32_t color /* = 0x186a64ff */)
{
    int idx = (y * rowLength + x) * 4;
    pixels[idx++] = color >> 24; //b
    pixels[idx++] = color >> 16; //g
    pixels[idx++] = color >> 8;  //r
    pixels[idx] = color;         //a
}

void    GameCore::fillBackground(std::uint8_t *pixels)
{
    for (int y = 0; y < m_height; y++)
        for (int x = 0; x < m_width; x++)
            setPixelToPixelArray(x, y, pixels, m_width);
}

void    GameCore::insertBlockToScene(int bx, int by, std::uint8_t *block, std::uint8_t *scene)
{
    for (int y = 0, nextRow = 0; y < BLOCK_SIZE; y++)
    {
        int idx = ((by + y) * m_width + bx) * 4;
        for (int i = nextRow; i < nextRow + BLOCK_SIZE * 4; i += 4)
            if (block[i + 3])
            {
                scene[idx++] = block[i];
                scene[idx++] = block[i + 1];
                scene[idx++] = block[i + 2];
                scene[idx++] = block[i + 3];
            }
            else
                idx += 4;

        nextRow += BLOCK_SIZE * 4;
    }
}

void    GameCore::insertElements(std::uint8_t *pixels)
{
    //insert obstacles, targets, snake
    for (auto e: obstacles)
        if (e->isVisible)
            insertBlockToScene(e->x, e->y, e->pxls, pixels);

    // if (target->isVisible)
    insertBlockToScene(target->x, target->y, target->pxls, pixels);
    if (bonusTarget->isVisible)
        insertBlockToScene(bonusTarget->x, bonusTarget->y, bonusTarget->pxls, pixels);
    // for (auto e: targets)
    //     if (e->isVisible)
    //         insertBlockToScene(e->x, e->y, e->pxls, pixels);

    for (auto e: snake)
        insertBlockToScene(e->x, e->y, e->pxls, pixels);
}

void    GameCore::initElements()
{
    //init obstacles
    //init targets
    obstacles.push_back(new Block(3*BLOCK_SIZE, 3*BLOCK_SIZE, true, Type::Obstacle, obstacle_pixels_map));
    obstacles.push_back(new Block(1*BLOCK_SIZE, 1*BLOCK_SIZE, true, Type::Obstacle, obstacle_pixels_map));
    obstacles.push_back(new Block(6*BLOCK_SIZE, 13*BLOCK_SIZE, true, Type::Obstacle, obstacle_pixels_map));
    obstacles.push_back(new Block(16*BLOCK_SIZE, 3*BLOCK_SIZE, true, Type::Obstacle, obstacle_pixels_map));

    target = new Block(20*BLOCK_SIZE, 12*BLOCK_SIZE, true, Type::Target, target_pixels_map);
    bonusTarget = new Block(8*BLOCK_SIZE, 4*BLOCK_SIZE, false, Type::Target, target_pixels_map);
    // targets[0] = new Block(3*BLOCK_SIZE, 4*BLOCK_SIZE, true, Type::Target, target_pixels_map);
    // targets[1] = new Block(7*BLOCK_SIZE, 8*BLOCK_SIZE, true, Type::Target, target_pixels_map);
    // targets[2] = new Block(11*BLOCK_SIZE, 12*BLOCK_SIZE, true, Type::Target, target_pixels_map);
    // targets[3] = new Block(23*BLOCK_SIZE, 18*BLOCK_SIZE, true, Type::Target, target_pixels_map);

    int halfSize = BLOCK_SIZE * (BLOCKS_PER_SIDE / 2 - 1);
    snake.push_back(new Block(halfSize + BLOCK_SIZE, halfSize, true, Type::Snake, getHeadPixels()));
    snake.push_back(new Block(halfSize, halfSize, true, Type::Snake, snake_body_pixels_map));
    snake.push_back(new Block(halfSize - BLOCK_SIZE, halfSize, true, Type::Snake, snake_body_pixels_map));
    snake.push_back(new Block(halfSize - 2*BLOCK_SIZE, halfSize, true, Type::Snake, snake_body_pixels_map));
}

void    GameCore::updateSnake(int nx, int ny)
{
    snake[0]->pxls = getHeadPixels();

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

void    GameCore::updateTarget(Block *m_target)
{
    int nextX = rand() % BLOCKS_PER_SIDE * BLOCK_SIZE;
    int nextY = rand() % BLOCKS_PER_SIDE * BLOCK_SIZE;
    while(checkObstacles(nextX, nextY))
    {
        nextX = rand() % BLOCKS_PER_SIDE * BLOCK_SIZE;
        nextY = rand() % BLOCKS_PER_SIDE * BLOCK_SIZE;
    }
    m_target->x = nextX;
    m_target->y = nextY;
}

void    GameCore::increaseSnake(int nx, int ny)
{
    snake.push_back(new Block(nx, ny, true, Type::Snake, snake_body_pixels_map));
}

//		1
//	2	0	3
//		4
//a w s d
std::uint8_t    *GameCore::getImage(std::uint8_t *pixels)
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
            nextX += (snake[0]->x + BLOCK_SIZE >= m_width) ? 0 : BLOCK_SIZE;
            break;
        case (4):
            nextY += (snake[0]->y + BLOCK_SIZE >= m_height) ? 0 : BLOCK_SIZE;
            break;
    }

    if (checkTarget(nextX, nextY, target))
    {
        sound.soundEat();
        increaseSnake(nextX, nextY);
        updateTarget(target);
    }

    if (checkTarget(nextX, nextY, bonusTarget))
    {
        sound.soundEat();
        increaseSnake(nextX, nextY);
        updateTarget(bonusTarget);
        bonusTarget->isVisible = false;
    }

    if (checkObstacles(nextX, nextY))
    {
        sound.endGame();
        exit(0);
    }
    else
        updateSnake(nextX, nextY);

    // std::cout << "snX " << snake[0]->x << " snY " << snake[0]->y << " ";
    //TODO: add frame to window
    fillBackground(pixels);
    insertElements(pixels);
    return pixels;
}

std::uint8_t    *GameCore::getHeadPixels()
{
    switch (direction)
    {
        case 1:
            return snake_h_north_pixels_map;
        case 4:
            return snake_h_south_pixels_map;
        case 2:
            return snake_h_west_pixels_map;
        case 3:
            return snake_h_east_pixels_map;
        default:
            return snake_h_north_pixels_map;
    }
}

bool    GameCore::checkTarget(int x, int y, Block* target)
{
    if (target->isVisible && target->x == x && target->y == y)
        return true;
    else
        return false;
}

bool    GameCore::checkObstacles(int x, int y)
{
    // Check for a collision of a snake with obstacles
    for (auto each: obstacles)
        if (each->x == x && each->y == y)
            return true;
    // Check for a collision of a snake with its own tail
    for(int i = 1; i < snake.size(); i++)
        if(snake[0]->x == snake[i]->x && snake[0]->y == snake[i]->y)
            return true;
    // Check for a collision of a snake with wall
    if (x == -1 || x == m_width || y == -1 || y == m_height)
        return true;

    return false;
}

// template <std::size_t OBJSIZE, std::size_t TARGETSSIZE>
// void    GameCore::gameLoop(GUIDisplay &disp,
//                            std::array<Block*, OBJSIZE> &obstacles
//                            std::array<Block*, TARGETSSIZE> &targets)
// {
//     int event = 0;
//     std::uint8_t pixels[ARRAY_SIZE];

//     while (event > -1) {
//         event = disp.run(getImage(event, pixels));
//         std::cout << "debug: libreturn: " << event << std::endl;
//     }
// }

void    GameCore::getLib(int libNumber)
{
    std::cout << "GameCore::getLib 1" << std::endl;
    currentLib = libNumber;
    if (libNumber == 10)
        lib_discr = load_lib("GLFWdl/glfwwrapper.so");
    else if (libNumber == 20)
        lib_discr = load_lib("SFMLdl/sfmlwrapper.so");
    else if (libNumber == 30)
        lib_discr = load_lib("SDL2dl/sdl2wrapper.so");

    std::function<GUIDisplay*(int, int)> create_wrapper((GUIDisplay*(*)(int, int))dlsym(lib_discr, "create_wrapper"));

    disp = create_wrapper(m_width, m_height);

    std::cout << "GameCore::getLib 2" << std::endl;
}

void	GameCore::run()
{
    std::cout << "GameCore::run 1_1" << std::endl;

    initElements();
    getLib(20);
    int   periodForBonus = 0;
    Timer           timer;
    timer.setTimeScale(0.2f);//TODO: replace by value of mandatory's requiroment
    std::uint8_t m_pixels[m_width * m_height * 4];
    std::function<void(GUIDisplay*)> release_wrapper((void(*)(GUIDisplay*))dlsym(lib_discr, "release_wrapper"));
    sound.startGame();
    while (direction)
    {
        timer.tick();
        if (timer.deltaTime() >= timer.getTimeScale())
        {
            if ((lastDirection == 3 && direction == 2) || (lastDirection == 2 && direction == 3) ||
                (lastDirection == 1 && direction == 4) || (lastDirection == 4 && direction == 1))
                direction = lastDirection;
            else
                lastDirection = direction;
            timer.reset();
            disp->render(getImage(m_pixels));
            sound.soundStep();
            periodForBonus++;
        }
        if (periodForBonus == 30)
            bonusTarget->isVisible = true;
        else if (periodForBonus == 60)
        {
            periodForBonus = 0;
            bonusTarget->isVisible = false;
            updateTarget(bonusTarget);
        }
        int tmp = disp->getEvent();
        if (tmp >= 1 && tmp <= 4)
            direction = tmp;
        else if (tmp >= 10 && tmp <= 30 && tmp != currentLib)
        {
            release_wrapper(disp);
            currentLib = tmp;
            getLib(tmp);
        }
        else if (tmp == 0)
        {
            release_wrapper(disp);
            exit(0);
        }
    }
    std::cout << "GameCore::run 2" << std::endl;
}

//TODO:create separeted object for loader or functor or smth else
//TODO: adjust realese resourses
void *GameCore::load_lib(std::string libname)
{
    void *lib_discriptor;

	lib_discriptor = dlopen(libname.data(), RTLD_LAZY);

    if (!lib_discriptor)
	{
		printf("***ERROR***\ndescriptor is not present: %s\n***********\n", dlerror());
		exit(1);
	}

    return lib_discriptor;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Debug functions																			    //
//////////////////////////////////////////////////////////////////////////////////////////////////

int GameCore::pixToInt(int x, int y, int rowWidth, std::uint8_t *pixels)
{
    int idx = (y * rowWidth + x) * 4;
    // return ((pixels[idx + 3] << 24) +
    //         (pixels[idx + 2] << 16) +
    //         (pixels[idx + 1] << 8) + (pixels[idx]));
    //for sfml:
    return ((pixels[idx] << 24) +
            (pixels[idx + 1] << 16) +
            (pixels[idx + 2] << 8) +
            (pixels[idx + 3]));
}