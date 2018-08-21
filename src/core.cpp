#include <dlfcn.h>
#include <iostream>
#include <functional>
#include <unistd.h>
#include "core.h"
#include "timer.h"
//#include "pngreader.h"

GameCore::GameCore() : direction(3), lastDirection(3)
{
    //TODO: temporary block start:
    fillPixelsToPixelsMap(target_pixels_map, 0xf0ff0fFF);
    fillPixelsToPixelsMap(obstacle_pixels_map, 0x000000FF);
    // fillPixelsToPixelsMap(snake_pixels_map, 0x00ff00FF);

    // std::cout << "before pr\n";
    // PngReader pr;
    // std::cout << "after pr\n";
    // snake_pixels_map = pr.readPixels((char*)"snake_body_simple.png");
    // std::cout << "after read pr\n";

    // for (int i = 0; i < 16*16*4; i++)
    //     std::cout << (int)snake_pixels_map[i] << ", ";
    // std::cout << std::endl;
    // snake_pixels_map = [this]() {
    //     uint32_t value = 0x2c8e40FF;
    //     std::uint8_t *px = new std::uint8_t[16*16*4];
    //     fillPixelsToPixelsMap(uint32_t)*(px, value);
    //     for (int y = 2; y < 14; y++)
    //     {
    //         for (int x = 2; x < 14; x++)
    //         {
    //             std::cout << "color & 0xff: " << (value & 0xFF) << std::endl;
    //             std::cout << "debug: snake: "<<value << "\n";
    //             std::cout << "debug: r: " << (value >> 24 & 0xFF)
    //                             << " g: " << (value >> 16 & 0xFF)
    //                             << " b: " << (value >> 8 & 0xFF) << "\n";
    //         }
    //     }
    //     return px;
    // }();
    snake_pixels_map = [this]() {
        std::uint8_t *px = new uint8_t[16*16*4];
        for (int i = 0, idx = 0; i < 16*16*4; i+=4, idx++)
        {
            uint32_t tmp[] = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
0,  0,  0,  3973279743U,  3973279743U,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
0,  0,  0,  3973279743U,  3973279743U,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  1936946175,  1936946175,  0,  0,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  1936946175,  1936946175,  0,  0,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};

            union {
                uint32_t val;
                uint8_t pix[4];
            };
            val = tmp[idx];
            // std::cout << val << " " << (int)pix[0]<< " " << (int)pix[1]<< " " << (int)pix[2]<< " " << (int)pix[3] << " ";
            px[i+0] = pix[2];//b
            px[i+1] = pix[1];//g
            px[i+2] = pix[0];//r
            px[i+3] = pix[3];
            // std::cout << " |" << (int)px[i+0]<< " " << (int)px[i+1]<< " " << (int)px[i+2]<< " " << (int)px[i+3] << "| ";
        }

        return px;
    }();
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

//uses to fill pixel arrays for blocks
void    GameCore::fillPixelsToPixelsMap(std::uint8_t *px, uint32_t color)
{
    for (int y = 0; y < BLOCK_SIZE; y++)
        for (int x = 0; x < BLOCK_SIZE; x++)
            setPixelToPixelArray(x, y, px, BLOCK_SIZE, color);
}

void    GameCore::setPixelToPixelArray(int x, int y, std::uint8_t *pixels,
                                       int rowLength, uint32_t color /* = 0xbbc59eff */)
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

void    GameCore::fillBackground(std::uint8_t *pixels)
{
    for (int y = 0; y < WIDTH_HEIGTH; y++)
        for (int x = 0; x < WIDTH_HEIGTH; x++)
            setPixelToPixelArray(x, y, pixels, WIDTH_HEIGTH);
}

void    GameCore::insertBlockToScene(int bx, int by, std::uint8_t *block, std::uint8_t *scene)
{
    for (int y = 0; y < BLOCK_SIZE; y++)
    {
        int idx = ((by + y) * WIDTH_HEIGTH + bx) * 4;
        for (int i = 0; i < BLOCK_SIZE * 4; i++)
            scene[idx++] = block[i];
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
    obstacles[0] = new Block(3*BLOCK_SIZE, 3*BLOCK_SIZE, true, Type::Obstacle, obstacle_pixels_map);
    obstacles[1] = new Block(1*BLOCK_SIZE, 1*BLOCK_SIZE, true, Type::Obstacle, obstacle_pixels_map);
    obstacles[2] = new Block(6*BLOCK_SIZE, 13*BLOCK_SIZE, true, Type::Obstacle, obstacle_pixels_map);
    obstacles[3] = new Block(16*BLOCK_SIZE, 3*BLOCK_SIZE, true, Type::Obstacle, obstacle_pixels_map);

    target = new Block(20*BLOCK_SIZE, 12*BLOCK_SIZE, true, Type::Target, target_pixels_map);
    bonusTarget = new Block(8*BLOCK_SIZE, 4*BLOCK_SIZE, false, Type::Target, target_pixels_map);
    // targets[0] = new Block(3*BLOCK_SIZE, 4*BLOCK_SIZE, true, Type::Target, target_pixels_map);
    // targets[1] = new Block(7*BLOCK_SIZE, 8*BLOCK_SIZE, true, Type::Target, target_pixels_map);
    // targets[2] = new Block(11*BLOCK_SIZE, 12*BLOCK_SIZE, true, Type::Target, target_pixels_map);
    // targets[3] = new Block(23*BLOCK_SIZE, 18*BLOCK_SIZE, true, Type::Target, target_pixels_map);

    int halfSize = BLOCK_SIZE * (BLOCKS_PER_SIDE / 2 - 1);
    snake.push_back(new Block(halfSize + BLOCK_SIZE, halfSize, true, Type::Snake, snake_pixels_map));
    snake.push_back(new Block(halfSize, halfSize, true, Type::Snake, snake_pixels_map));
    snake.push_back(new Block(halfSize - BLOCK_SIZE, halfSize, true, Type::Snake, snake_pixels_map));
    snake.push_back(new Block(halfSize - 2*BLOCK_SIZE, halfSize, true, Type::Snake, snake_pixels_map));
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

void    GameCore::updateTarget(Block **m_target)
{
    int nextX = rand() % BLOCKS_PER_SIDE * BLOCK_SIZE;
    int nextY = rand() % BLOCKS_PER_SIDE * BLOCK_SIZE;
    while(checkObstacles(nextX, nextY, obstacles))
    {
        nextX = rand() % BLOCKS_PER_SIDE * BLOCK_SIZE;
        nextY = rand() % BLOCKS_PER_SIDE * BLOCK_SIZE;
    }
    (*m_target)->x = nextX;
    (*m_target)->y = nextY;
}

void    GameCore::increaseSnake(int nx, int ny)
{
    snake.insert(snake.end(), new Block(nx, ny, true, Type::Snake, snake_pixels_map));
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
            nextX += (snake[0]->x + BLOCK_SIZE >= WIDTH_HEIGTH) ? 0 : BLOCK_SIZE;
            break;
        case (4):
            nextY += (snake[0]->y + BLOCK_SIZE >= WIDTH_HEIGTH) ? 0 : BLOCK_SIZE;
            break;
    }

    if (checkTarget(nextX, nextY, target))
    {
        increaseSnake(nextX, nextY);
        updateTarget(&target);
    }

    if (checkTarget(nextX, nextY, bonusTarget))
    {
        increaseSnake(nextX, nextY);
        updateTarget(&bonusTarget);
        bonusTarget->isVisible = false;
    }

    if (!checkObstacles(nextX, nextY, obstacles))
        updateSnake(nextX, nextY);

    // std::cout << "snX " << snake[0]->x << " snY " << snake[0]->y << " ";
    //TODO: add frame to window
    fillBackground(pixels);
    insertElements(pixels);
    return pixels;
}

template <std::size_t ARRSIZE>
bool    GameCore::checkTargets(int x, int y, std::array<Block*, ARRSIZE> &targets)
{
    for (auto each: targets)
        if (each->isVisible && each->x == x && each->y == y)
        {
            each->isVisible = false;
            return true;
        }

    return false;
}

bool    GameCore::checkTarget(int x, int y, Block* target)
{
    if (target->isVisible && target->x == x && target->y == y)
        return true;
    else    
        return false;
}

template <std::size_t ARRSIZE>
bool    GameCore::checkObstacles(int x, int y, std::array<Block*, ARRSIZE> &obstacles)
{
    for (auto each: obstacles)
        if (each->x == x && each->y == y)
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

void	GameCore::run()
{
    // GUIDisplay *disp = new SfmlWrapper();
    // GUIDisplay *disp = new SDL2Wrapper();

    //TODO: replace by functor?
    void *lib_discr = load_lib("GLFWdl/glfwwrapper.so");
    // void *lib_discr = load_lib("SFMLdl/sfmlwrapper.so");
    // void *lib_discr = load_lib("SDL2dl/sdl2wrapper.so");
    std::function<GUIDisplay*(int, int)> create_wrapper((GUIDisplay*(*)(int, int))dlsym(lib_discr, "create_wrapper"));
    std::function<void(GUIDisplay*)> release_wrapper((void(*)(GUIDisplay*))dlsym(lib_discr, "release_wrapper"));

    // GUIDisplay *disp = new GlfwWrapper();
    GUIDisplay *disp = create_wrapper(5, 5);

    Timer timer;
    int   periodForBonus = 0;

    timer.setTimeScale(0.2f);//TODO: replace by value of mandatory's requiroment

	// std::array<Block*, OBSTACLES_QUANTITY> obstacles;
	// std::array<Block*, TARGETS_QUANTITY> targets;

    initElements();

    std::uint8_t pixels[ARRAY_SIZE];

    sound.soundStartGame();
    while (direction)
    {
        timer.tick();
        if (timer.deltaTime() >= timer.getTimeScale())
        {
            timer.reset();
            disp->render(getImage(pixels));
            periodForBonus++;
        }
        if(periodForBonus == 30)
            bonusTarget->isVisible = true;
        else if (periodForBonus == 60)
        {
            periodForBonus = 0;
            bonusTarget->isVisible = false;
            updateTarget(&bonusTarget);
        }
        direction = disp->getEvent();
        if((lastDirection == 3 && direction == 2) || (lastDirection == 2 && direction == 3) ||
           (lastDirection == 1 && direction == 4) || (lastDirection == 4 && direction == 1))
            direction = lastDirection;
        else
            lastDirection = direction;

        // // std::cout << "debug: libreturn: " << direction << std::endl;
    }

    // gameLoop(disp, allElenents, obstacles, targets);
    release_wrapper(disp);
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

int GameCore::pixToInt(int x, int y, std::uint8_t *pixels)
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