#include <dlfcn.h>
#include <iostream>
#include <functional>
#include <unistd.h>
#include "core.h"
#include "timer.h"
#include "imgloader.h"
#include "audiowrapper.h"

GameCore::GameCore(int w, int h) :
    direction_1(3),
	direction_2(8),
    lastDirection_1(3),
	lastDirection_2(8),
    horizontBlocksNum(w),
    verticalBlocksNum(h),
    m_width(w * BLOCK_SIZE),
    m_height(h * BLOCK_SIZE)
{
    //TODO: temporary block start:
    fillPixelsToPixelsMap(obstacle_pixels_map, 0xFF0000FF);

    std::function<AudioWrapper*()> createAudioWrapper(reinterpret_cast<AudioWrapper*(*)()>(dlsym(loadLib("AudioWrapper/audiowrapper.so"), "createAudioWrapper")));
    sound = createAudioWrapper();

    initElements();
}

GameCore::~GameCore()
{
    //TODO: free block's arrays content
    for (auto e: obstacles)
        delete e;
    for (auto e: snake_1)
        delete e;
    for (auto e: targetPixelMaps)
        delete e;
    delete target;
    delete bonusTarget;
    // delete target_pixels_map;
    // delete obstacle_pixels_map;
    delete snake_body_pixels_map;
    delete snake_h_north_pixels_map;
    delete snake_h_south_pixels_map;
    delete snake_h_west_pixels_map;
    delete snake_h_east_pixels_map;

    std::function<void(AudioWrapper*)> releaseAudioWrapper(reinterpret_cast<void(*)(AudioWrapper*)>(dlsym(loadLib("AudioWrapper/audiowrapper.so"), "releaseAudioWrapper")));
    releaseAudioWrapper(sound);
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

    insertBlockToScene(target->x, target->y, target->pxls, pixels);

    if (bonusTarget->isVisible)
        insertBlockToScene(bonusTarget->x, bonusTarget->y, bonusTarget->pxls, pixels);

    for (auto e: snake_1)
        insertBlockToScene(e->x, e->y, e->pxls, pixels);

	for (auto e: snake_2)
        insertBlockToScene(e->x, e->y, e->pxls, pixels);
}

void    GameCore::initElements()
{
    //load textures
    void *imageLoaderDiscriptor = loadLib("IMGLoader/imgloader.so");
    std::function<IMGLoader*()> createImgLoader(reinterpret_cast<IMGLoader*(*)()>(dlsym(imageLoaderDiscriptor, "createImgLoader")));
    std::function<void(IMGLoader*)> releaseImgLoader(reinterpret_cast<void(*)(IMGLoader*)>(dlsym(imageLoaderDiscriptor, "releaseImgLoader")));
    IMGLoader *imloader = createImgLoader();

    snake_body_pixels_map = imloader->getPixelMap("assets/body_48.png");
    //TODO: replace by simple snake head and body, scaled to 48 pix
    snake_h_north_pixels_map = imloader->getPixelMap("assets/head_north_48.png");
    snake_h_south_pixels_map = imloader->getPixelMap("assets/head_north_48.png");
    snake_h_west_pixels_map = imloader->getPixelMap("assets/head_north_48.png");
    snake_h_east_pixels_map = imloader->getPixelMap("assets/head_north_48.png");

    targetPixelMaps[0] = imloader->getPixelMap("assets/apple_red_48.png");
    targetPixelMaps[1] = imloader->getPixelMap("assets/apple_green_48.png");
    targetPixelMaps[2] = imloader->getPixelMap("assets/cherry_48.png");
    targetPixelMaps[3] = imloader->getPixelMap("assets/icecream_48.png");
    targetPixelMaps[4] = imloader->getPixelMap("assets/pie_48.png");
    targetPixelMaps[5] = imloader->getPixelMap("assets/strawberry_48.png");

    releaseImgLoader(imloader);

    //init obstacles
    auto getRandCoordinate = [](int distance) {
        // auto randnum = rand() % distance;
        // auto ret =  BLOCK_SIZE * randnum;
        // std::cout << "rand coordinate, dist: " << distance << ", randnum: " << randnum << ", ret: " << ret << std::endl;
        return (rand() % distance) * BLOCK_SIZE;
    };

//TODO: need to resolve collision (into getRandCoordinate) between obstacles positions and initial snake's positions and
//      directions to prevent an issue, when snakes appear in face with obstacle. Also need to resolve collisions between
//      currently inserted items and new items (for new obstacles, targets)
    obstacles.push_back(new Block(getRandCoordinate(horizontBlocksNum), getRandCoordinate(verticalBlocksNum), true, Type::Obstacle, obstacle_pixels_map));
    obstacles.push_back(new Block(getRandCoordinate(horizontBlocksNum), getRandCoordinate(verticalBlocksNum), true, Type::Obstacle, obstacle_pixels_map));
    obstacles.push_back(new Block(getRandCoordinate(horizontBlocksNum), getRandCoordinate(verticalBlocksNum), true, Type::Obstacle, obstacle_pixels_map));
    obstacles.push_back(new Block(getRandCoordinate(horizontBlocksNum), getRandCoordinate(verticalBlocksNum), true, Type::Obstacle, obstacle_pixels_map));
    obstacles.push_back(new Block(getRandCoordinate(horizontBlocksNum), getRandCoordinate(verticalBlocksNum), true, Type::Obstacle, obstacle_pixels_map));

    //init targets
  //TODO: the same as above
    target = new Block(getRandCoordinate(horizontBlocksNum), getRandCoordinate(verticalBlocksNum), true, Type::Target, targetPixelMaps[rand() % 6]);
    bonusTarget = new Block(getRandCoordinate(horizontBlocksNum), getRandCoordinate(verticalBlocksNum), false, Type::Target, targetPixelMaps[rand() % 6]);

    //init snakes
    int horizontalHalfSize = BLOCK_SIZE * (horizontBlocksNum / 2 - 1);
    int verticalHalfSize = BLOCK_SIZE * (verticalBlocksNum / 2 - 1);

    int topIndention = BLOCK_SIZE;
    snake_1.push_back(new Block(horizontalHalfSize + BLOCK_SIZE, topIndention, true, Type::Snake, getHeadPixels(1)));
    snake_1.push_back(new Block(horizontalHalfSize, topIndention, true, Type::Snake, snake_body_pixels_map));
    snake_1.push_back(new Block(horizontalHalfSize - BLOCK_SIZE, topIndention, true, Type::Snake, snake_body_pixels_map));
    snake_1.push_back(new Block(horizontalHalfSize - 2*BLOCK_SIZE, topIndention, true, Type::Snake, snake_body_pixels_map));

    int bottomIndention = m_height - 2 * BLOCK_SIZE;
	snake_2.push_back(new Block(horizontalHalfSize + BLOCK_SIZE, bottomIndention, true, Type::Snake, getHeadPixels(2)));
    snake_2.push_back(new Block(horizontalHalfSize, bottomIndention, true, Type::Snake, snake_body_pixels_map));
    snake_2.push_back(new Block(horizontalHalfSize - BLOCK_SIZE, bottomIndention, true, Type::Snake, snake_body_pixels_map));
    snake_2.push_back(new Block(horizontalHalfSize - 2*BLOCK_SIZE, bottomIndention, true, Type::Snake, snake_body_pixels_map));
}

void    GameCore::updateSnake(int nx, int ny, std::vector<Block*> snake, int snakeNumber)
{
    snake[0]->pxls = getHeadPixels(snakeNumber);

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

void    GameCore::updateTarget(Block *target)
{
    int nextX = rand() % horizontBlocksNum * BLOCK_SIZE;
    int nextY = rand() % verticalBlocksNum * BLOCK_SIZE;
    while (checkObstacles(nextX, nextY, snake_1) && checkObstacles(nextX, nextY, snake_2))
    {
        nextX = rand() % horizontBlocksNum * BLOCK_SIZE;
        nextY = rand() % verticalBlocksNum * BLOCK_SIZE;
    }
    target->x = nextX;
    target->y = nextY;
    target->pxls = targetPixelMaps[rand() % 6];
}

void    GameCore::increaseSnake(int nx, int ny, int snakeNumber)
{
	if (snakeNumber == 1)
    	snake_1.push_back(new Block(nx, ny, true, Type::Snake, snake_body_pixels_map));
	else
		snake_2.push_back(new Block(nx, ny, true, Type::Snake, snake_body_pixels_map));
}

//		1
//	2	0	3
//		4
//a w s d
std::uint8_t    *GameCore::getImage(std::uint8_t *pixels)
{
    int nextX_1 = snake_1[0]->x;
    int nextY_1 = snake_1[0]->y;
	int nextX_2 = snake_2[0]->x;
    int nextY_2 = snake_2[0]->y;

    switch (direction_1)
    {
        case (1):
            nextY_1 -= (snake_1[0]->y - BLOCK_SIZE < 0) ? 0 : BLOCK_SIZE;
            break;
        case (2):
            nextX_1 -= (snake_1[0]->x - BLOCK_SIZE < 0) ? 0 : BLOCK_SIZE;
            break;
        case (3):
            nextX_1 += (snake_1[0]->x + BLOCK_SIZE >= m_width) ? 0 : BLOCK_SIZE;
            break;
        case (4):
            nextY_1 += (snake_1[0]->y + BLOCK_SIZE >= m_height) ? 0 : BLOCK_SIZE;
            break;
    }

	switch (direction_2)
    {
        case (5):
            nextY_2 -= (snake_2[0]->y - BLOCK_SIZE < 0) ? 0 : BLOCK_SIZE;
            break;
        case (7):
            nextX_2 -= (snake_2[0]->x - BLOCK_SIZE < 0) ? 0 : BLOCK_SIZE;
            break;
        case (8):
            nextX_2 += (snake_2[0]->x + BLOCK_SIZE >= m_width) ? 0 : BLOCK_SIZE;
            break;
        case (6):
            nextY_2 += (snake_2[0]->y + BLOCK_SIZE >= m_height) ? 0 : BLOCK_SIZE;
            break;
    }

    if (checkTarget(nextX_1, nextY_1, target))
        increaseSnake(nextX_1, nextY_1, 1);
	else if (checkTarget(nextX_2, nextY_2, target))
        increaseSnake(nextX_2, nextY_2, 2);

    if (checkTarget(nextX_1, nextY_1, bonusTarget))
    {
        increaseSnake(nextX_1, nextY_1, 1);
        bonusTarget->isVisible = false;
    }
	else if (checkTarget(nextX_2, nextY_2, bonusTarget))
    {
        increaseSnake(nextX_2, nextY_2, 2);
        bonusTarget->isVisible = false;
    }

    if (checkObstacles(nextX_1, nextY_1, snake_1))
    {
        // sound->endGame();
        // exit(0);
    }
	else if (checkObstacles(nextX_2, nextY_2, snake_2))
	{

	}
    else
	{
        updateSnake(nextX_1, nextY_1, snake_1, 1);
		updateSnake(nextX_2, nextY_2, snake_2, 2);
	}

    //TODO: add frame to window
    fillBackground(pixels);
    insertElements(pixels);
    return pixels;
}

std::uint8_t    *GameCore::getHeadPixels(int snakeNumber)
{
	if (snakeNumber == 1)
		switch (direction_1)
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

	if (snakeNumber == 2)
		switch (direction_2)
		{
			case 5:
				return snake_h_north_pixels_map;
			case 6:
				return snake_h_south_pixels_map;
			case 7:
				return snake_h_west_pixels_map;
			case 8:
				return snake_h_east_pixels_map;
			default:
				return snake_h_north_pixels_map;
		}
	return nullptr;
}

bool    GameCore::checkTarget(int x, int y, Block* target)
{
    if (target->isVisible && target->x == x && target->y == y)
	{
		sound->soundEat();
        updateTarget(target);
        return true;
	}
    else
        return false;
}

bool    GameCore::checkObstacles(int x, int y, std::vector<Block*> snake)
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

void    GameCore::getLib(int libNumber)
{
    std::cout << "GameCore::getLib: " << libNumber << std::endl;
    currentLib = libNumber;

    switch (libNumber)
    {
        case (10):
            lib_discr = loadLib("GLFWdl/glfwwrapper.so");
            break;
        case (20):
            lib_discr = loadLib("SFMLdl/sfmlwrapper.so");
            break;
        case (30):
            lib_discr = loadLib("SDL2dl/sdl2wrapper.so");
            break;
        default:
            lib_discr = loadLib("SFMLdl/sfmlwrapper.so");
    }

    std::function<GUIDisplay*(int, int)> create_wrapper(reinterpret_cast<GUIDisplay*(*)(int, int)>(dlsym(lib_discr, "create_wrapper")));

    disp = create_wrapper(m_width, m_height);

    std::cout << "GameCore::getLib, current lib: " << currentLib << std::endl;
}

void	GameCore::getDirection(std::function<void(GUIDisplay*)> release_wrapper)
{
	    int tmp = disp->getEvent();
        if (tmp >= 1 && tmp <= 4)
            direction_1 = tmp;
		else if (tmp >= 5 && tmp <= 8)
			direction_2 = tmp;
        else if (tmp >= 10 && tmp <= 30 && tmp != currentLib)
        {
            release_wrapper(disp);
            currentLib = tmp;
            getLib(tmp);
        }
        else if (tmp == 0)
        {
            release_wrapper(disp);
            exit(0);//TODO: we need to end game, nod just exit program
        }
}

void	GameCore::checkDirection()
{
	if ((lastDirection_1 == 3 && direction_1 == 2) || (lastDirection_1 == 2 && direction_1 == 3) ||
        (lastDirection_1 == 1 && direction_1 == 4) || (lastDirection_1 == 4 && direction_1 == 1))
        direction_1 = lastDirection_1;
    else
        lastDirection_1 = direction_1;

	if ((lastDirection_2 == 8 && direction_2 == 7) || (lastDirection_2 == 7 && direction_2 == 8) ||
        (lastDirection_2 == 5 && direction_2 == 6) || (lastDirection_2 == 6 && direction_2 == 5))
        direction_2 = lastDirection_2;
    else
        lastDirection_2 = direction_2;
}

void	GameCore::run()
{
    getLib(10);
    int             periodForBonus = 0;
    Timer           timer;
    std::uint8_t    m_pixels[m_width * m_height * 4];
    std::function<void(GUIDisplay*)> release_wrapper(reinterpret_cast<void(*)(GUIDisplay*)>(dlsym(lib_discr, "release_wrapper")));

    timer.setTimeScale(.3f);//TODO: replace by value of mandatory's requiroment

    sound->startGame();
    while (direction_1)
    {
        timer.tick();
        if (timer.deltaTime() >= timer.getTimeScale())
        {
            checkDirection();
            timer.reset();
            disp->render(getImage(m_pixels));
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
		getDirection(release_wrapper);
    }
}

//TODO:create separeted object for loader or functor or smth else
//TODO: adjust realese resourses
void *GameCore::loadLib(std::string libname)
{
    void *lib_discriptor;

	lib_discriptor = dlopen(libname.data(), RTLD_LAZY);

    if (!lib_discriptor)
	{
		std::cout << "*** DINAMYC LIBRARY LOADING ERROR ***\ndescriptor is not present: "
                  << dlerror() << std::endl;
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