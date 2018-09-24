#include <dlfcn.h>
#include <iostream>
#include <unistd.h>
#include "core.h"
#include "timer.h"
#include "imgloader.h"

GameCore::GameCore(int w, int h, int mode) :
    gameMode(mode),
    direction_1(3),
    direction_2(8),
    lastDirection_1(3),
    lastDirection_2(8),
    horizontBlocksNum(w),
    verticalBlocksNum(h),
    m_width(w * BLOCK_SIZE),
    m_height(h * BLOCK_SIZE),
    scoreBlockWidth(w * BLOCK_SIZE),
    scoreBlockHeight(96),
    scoreCount(0),
    increaseSpeed(false)
{
    initElements();
}

GameCore::~GameCore()
{
    delete target;
    delete bonusTarget;
    for (auto e: obstacles)
        delete e;
    for (auto e: snake_1)
        delete e;
    for (auto e: snake_2)
        delete e;
    for (auto e: targetPixelMaps)
        delete[] e;
    for (auto e: obstaclePixelMaps)
        delete[] e;
    for (auto e: numbersPixelMaps)
        delete[] e;
    delete[] snake_body_pixels_map;
    delete[] snake_h_north_pixels_map;
    delete[] snake_h_south_pixels_map;
    delete[] snake_h_west_pixels_map;
    delete[] snake_h_east_pixels_map;
    delete[] snake_2_body_pixels_map;
    delete[] snake_2_h_north_pixels_map;
    delete[] snake_2_h_south_pixels_map;
    delete[] snake_2_h_west_pixels_map;
    delete[] snake_2_h_east_pixels_map;
    delete[] startGamePixelMap;
    delete[] fieldPixelMap;
    delete[] scorePixelMap;

    std::function<void(AudioWrapper*)> releaseAudioWrapper(reinterpret_cast<void(*)(AudioWrapper*)>(dlsym(loadLib("AudioWrapper/audiowrapper.so"), "releaseAudioWrapper")));
    releaseAudioWrapper(sound);
}

void    GameCore::initElements()
{
    //init audio lib
    std::function<AudioWrapper*()> createAudioWrapper(reinterpret_cast<AudioWrapper*(*)()>(dlsym(loadLib("AudioWrapper/audiowrapper.so"), "createAudioWrapper")));
    sound = createAudioWrapper();

    //load textures
    void *imageLoaderDiscriptor = loadLib("IMGLoader/imgloader.so");
    std::function<IMGLoader*()> createImgLoader(reinterpret_cast<IMGLoader*(*)()>(dlsym(imageLoaderDiscriptor, "createImgLoader")));
    std::function<void(IMGLoader*)> releaseImgLoader(reinterpret_cast<void(*)(IMGLoader*)>(dlsym(imageLoaderDiscriptor, "releaseImgLoader")));
    IMGLoader *imloader = createImgLoader();
    snake_body_pixels_map = imloader->getPixelMap("assets/body_48.png");
    snake_h_north_pixels_map = imloader->getPixelMap("assets/head_north_48.png");
    snake_h_south_pixels_map = imloader->getPixelMap("assets/head_south_48.png");
    snake_h_west_pixels_map = imloader->getPixelMap("assets/head_west_48.png");
    snake_h_east_pixels_map = imloader->getPixelMap("assets/head_east_48.png");

    snake_2_body_pixels_map = imloader->getPixelMap("assets/body_2_48.png");
    snake_2_h_north_pixels_map = imloader->getPixelMap("assets/head_2_north_48.png");
    snake_2_h_south_pixels_map = imloader->getPixelMap("assets/head_2_south_48.png");
    snake_2_h_west_pixels_map = imloader->getPixelMap("assets/head_2_west_48.png");
    snake_2_h_east_pixels_map = imloader->getPixelMap("assets/head_2_east_48.png");

    targetPixelMaps[0] = imloader->getPixelMap("assets/apple_red_48.png");
    targetPixelMaps[1] = imloader->getPixelMap("assets/apple_green_48.png");
    targetPixelMaps[2] = imloader->getPixelMap("assets/cherry_48.png");
    targetPixelMaps[3] = imloader->getPixelMap("assets/icecream_48.png");
    targetPixelMaps[4] = imloader->getPixelMap("assets/pie_48.png");
    targetPixelMaps[5] = imloader->getPixelMap("assets/strawberry_48.png");

    obstaclePixelMaps[0] = imloader->getPixelMap("assets/stones_1_48.png");
    obstaclePixelMaps[1] = imloader->getPixelMap("assets/stones_2_48.png");
    obstaclePixelMaps[2] = imloader->getPixelMap("assets/stones_3_48.png");
    obstaclePixelMaps[3] = imloader->getPixelMap("assets/stones_4_48.png");
    obstaclePixelMaps[4] = imloader->getPixelMap("assets/stones_5_48.png");

    startGamePixelMap = imloader->getPixelMap("assets/game_opening.png");
    endGamePixelMap = imloader->getPixelMap("assets/game_over.png");
    fieldPixelMap = imloader->getPixelMap("assets/field.png");
    scorePixelMap = imloader->getPixelMap("assets/score_272_96.png");

    numbersPixelMaps[0] = imloader->getPixelMap("assets/zero_48_96.png");
    numbersPixelMaps[1] = imloader->getPixelMap("assets/one_48_96.png");
    numbersPixelMaps[2] = imloader->getPixelMap("assets/two_48_96.png");
    numbersPixelMaps[3] = imloader->getPixelMap("assets/three_48_96.png");
    numbersPixelMaps[4] = imloader->getPixelMap("assets/four_48_96.png");
    numbersPixelMaps[5] = imloader->getPixelMap("assets/five_48_96.png");
    numbersPixelMaps[6] = imloader->getPixelMap("assets/six_48_96.png");
    numbersPixelMaps[7] = imloader->getPixelMap("assets/seven_48_96.png");
    numbersPixelMaps[8] = imloader->getPixelMap("assets/eight_48_96.png");
    numbersPixelMaps[9] = imloader->getPixelMap("assets/nine_48_96.png");

    releaseImgLoader(imloader);

    //init snakes
    int horizontalHalfSize = BLOCK_SIZE * (horizontBlocksNum / 2 - 1);
    int verticalHalfSize = BLOCK_SIZE * (verticalBlocksNum / 2 - 1);

    int topIndention = (gameMode == 2) ? BLOCK_SIZE : verticalHalfSize;
    snake_1.push_back(new Block(horizontalHalfSize + BLOCK_SIZE, topIndention, true, getHeadPixels(1)));
    snake_1.push_back(new Block(horizontalHalfSize, topIndention, true, snake_body_pixels_map));
    snake_1.push_back(new Block(horizontalHalfSize - BLOCK_SIZE, topIndention, true, snake_body_pixels_map));
    snake_1.push_back(new Block(horizontalHalfSize - 2 * BLOCK_SIZE, topIndention, true, snake_body_pixels_map));

    int bottomIndention = m_height - 2 * BLOCK_SIZE;
    snake_2.push_back(new Block(horizontalHalfSize + BLOCK_SIZE, bottomIndention, true, getHeadPixels(2)));
    snake_2.push_back(new Block(horizontalHalfSize, bottomIndention, true, snake_2_body_pixels_map));
    snake_2.push_back(new Block(horizontalHalfSize - BLOCK_SIZE, bottomIndention, true, snake_2_body_pixels_map));
    snake_2.push_back(new Block(horizontalHalfSize - 2 * BLOCK_SIZE, bottomIndention, true, snake_2_body_pixels_map));

    //init obstacles
    obstacles.push_back(getBlock(horizontBlocksNum - 2, verticalBlocksNum - 2, obstaclePixelMaps[0], true));
    obstacles.push_back(getBlock(horizontBlocksNum - 2, verticalBlocksNum - 2, obstaclePixelMaps[1], true));
    obstacles.push_back(getBlock(horizontBlocksNum - 2, verticalBlocksNum - 2, obstaclePixelMaps[2], true));
    obstacles.push_back(getBlock(horizontBlocksNum - 2, verticalBlocksNum - 2, obstaclePixelMaps[3], true));
    obstacles.push_back(getBlock(horizontBlocksNum - 2, verticalBlocksNum - 2, obstaclePixelMaps[4], true));

    //init targets
    target = getBlock(horizontBlocksNum - 1, verticalBlocksNum - 1, targetPixelMaps[rand() % 6], true);
    bonusTarget = getBlock(horizontBlocksNum - 1, verticalBlocksNum - 1, targetPixelMaps[rand() % 6], true);
}

GameCore::Block   *GameCore::getBlock(int rangeX, int rangeY, std::uint8_t *blockPxls, bool isVisible)
{
    int x, y;

    getRandomCoordinates(x, y, rangeX, rangeY);

    return new Block(x, y, isVisible, blockPxls);
}

void    GameCore::getRandomCoordinates(int &x, int &y, int rangeX, int rangeY)
{
    do
    {
        x = (rand() % rangeX + 1) * BLOCK_SIZE;
        y = (rand() % rangeY + 1) * BLOCK_SIZE;
    }
    while (!isCoordinatesFree(x, y));
}

bool    GameCore::isCoordinatesFree(int &x, int &y)
{
    for (auto e: snake_1)
        if (e->x == x && e->y == y)
            return false;
    if (gameMode == 2)
        for (auto e: snake_2)
            if (e->x == x && e->y == y)
                return false;
    for (auto e: obstacles)
        if (e->x == x && e->y == y)
            return false;

    return true;
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

void    GameCore::fillBackground(std::uint8_t *pixels, int xFrom, int xTo, int yFrom, int yTo, uint32_t color /* = 0x186a64ff */)
{
    for (int y = yFrom; y < yTo; ++y)
        for (int x = xFrom; x < xTo; ++x)
            setPixelToPixelArray(x, y, pixels, m_width, color);
}

void    GameCore::insertBlockToScene(int sceneX, int sceneY, int blockWidth,
                                     int blockHeight, std::uint8_t *block, std::uint8_t *scene)
{
    for (int y = 0, nextRow = 0; y < blockHeight; ++y)
    {
        int idx = ((sceneY + y) * m_width + sceneX) * 4;
        for (int i = nextRow; i < nextRow + blockWidth * 4; i += 4)
            if (block[i + 3])
            {
                scene[idx++] = block[i];
                scene[idx++] = block[i + 1];
                scene[idx++] = block[i + 2];
                scene[idx++] = block[i + 3];
            }
            else
                idx += 4;

        nextRow += blockWidth * 4;
    }
}

void    GameCore::insertElements(std::uint8_t *pixels)
{
    //insert obstacles, targets, snake
    for (auto e: obstacles)
        if (e->isVisible)
            insertBlockToScene(e->x, e->y, BLOCK_SIZE, BLOCK_SIZE, e->pxls, pixels);

    insertBlockToScene(target->x, target->y, BLOCK_SIZE, BLOCK_SIZE, target->pxls, pixels);

    if (bonusTarget->isVisible)
        insertBlockToScene(bonusTarget->x, bonusTarget->y, BLOCK_SIZE, BLOCK_SIZE, bonusTarget->pxls, pixels);

    for (auto e: snake_1)
        insertBlockToScene(e->x, e->y, BLOCK_SIZE, BLOCK_SIZE, e->pxls, pixels);

    if (gameMode == 2)
    {
        for (auto e: snake_2)
            insertBlockToScene(e->x, e->y, BLOCK_SIZE, BLOCK_SIZE, e->pxls, pixels);
    }
}

void    GameCore::insertField(std::uint8_t *pixels)
{
    for (int y = 0; y < verticalBlocksNum; ++y)
        for (int x = 0; x < horizontBlocksNum; ++x)
            insertBlockToScene(x * BLOCK_SIZE, y * BLOCK_SIZE, 48, 48, fieldPixelMap, pixels);
}

void    GameCore::insertScore(std::uint8_t *pixels)
{
    insertBlockToScene(0, m_height, 272, 96, scorePixelMap, pixels);
    //fill the gap between 'score:' and score count
    fillBackground(pixels, 272, m_width - 192, m_height, m_height + 96, 0);
    insertScoreCount(pixels);
}

void    GameCore::insertScoreCount(std::uint8_t *pixels)
{
    std::array<std::uint8_t, 4> idx;
    int tmp = scoreCount;

    idx[3] = tmp % 10; tmp /= 10;
    idx[2] = tmp % 10; tmp /= 10;
    idx[1] = tmp % 10; tmp /= 10;
    idx[0] = tmp % 10; tmp /= 10;

    insertBlockToScene(m_width - 192, m_height, 48, 96, numbersPixelMaps[idx[0]], pixels);
    insertBlockToScene(m_width - 144, m_height, 48, 96, numbersPixelMaps[idx[1]], pixels);
    insertBlockToScene(m_width - 96, m_height, 48, 96, numbersPixelMaps[idx[2]], pixels);
    insertBlockToScene(m_width - 48, m_height, 48, 96, numbersPixelMaps[idx[3]], pixels);
}

void    GameCore::increaseSnake(int nx, int ny, int snakeNumber)
{
    if (snakeNumber == 1)
        snake_1.push_back(new Block(nx, ny, true, snake_body_pixels_map));
    else
        snake_2.push_back(new Block(nx, ny, true, snake_2_body_pixels_map));
}

void    GameCore::updateSnake(int nx, int ny, std::vector<Block*> snake, int snakeNumber)
{
    snake[0]->pxls = getHeadPixels(snakeNumber);

    int previousElementX = snake[0]->x;
    int previousElementY = snake[0]->y;
    int tmpX, tmpY;

    snake[0]->x = nx;
    snake[0]->y = ny;

    for (std::uint32_t idx = 1; idx < snake.size(); ++idx)
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
    getRandomCoordinates(target->x, target->y, horizontBlocksNum - 1, verticalBlocksNum - 1);
    target->pxls = targetPixelMaps[rand() % 6];
}

bool    GameCore::getImage(std::uint8_t *pixels)
{
    int nextX_1 = snake_1[0]->x;
    int nextY_1 = snake_1[0]->y;
    int nextX_2 = snake_2[0]->x;
    int nextY_2 = snake_2[0]->y;

    //get the coordinates of the next position
    switch (direction_1)
    {
        case (1):
            nextY_1 -= BLOCK_SIZE;
            break;
        case (2):
            nextX_1 -= BLOCK_SIZE;
            break;
        case (3):
            nextX_1 += BLOCK_SIZE;
            break;
        case (4):
            nextY_1 += BLOCK_SIZE;
            break;
    }

    if (gameMode == 2)
        switch (direction_2)
        {
            case (5):
                nextY_2 -= BLOCK_SIZE;
                break;
            case (7):
                nextX_2 -= BLOCK_SIZE;
                break;
            case (8):
                nextX_2 += BLOCK_SIZE;
                break;
            case (6):
                nextY_2 += BLOCK_SIZE;
                break;
        }

    //check the targets
    if (checkTarget(nextX_1, nextY_1, target))
        increaseSnake(nextX_1, nextY_1, 1);
    else if (gameMode == 2 && checkTarget(nextX_2, nextY_2, target))
        increaseSnake(nextX_2, nextY_2, 2);

    if (bonusTarget->isVisible)
    {
        if (checkTarget(nextX_1, nextY_1, bonusTarget))
        {
            increaseSnake(nextX_1, nextY_1, 1);
            bonusTarget->isVisible = false;
        }
        else if (gameMode == 2 && checkTarget(nextX_2, nextY_2, bonusTarget))
        {
            increaseSnake(nextX_2, nextY_2, 2);
            bonusTarget->isVisible = false;
        }
    }

    //check the obstacles
    if (checkObstacles(nextX_1, nextY_1, snake_1) ||
        (gameMode == 2 && checkObstacles(nextX_2, nextY_2, snake_2)))
    {
        sound->endGame();
        showingLoop(1.0f);
        return false;
    }
    else
    {
        updateSnake(nextX_1, nextY_1, snake_1, 1);
        if (gameMode == 2)
            updateSnake(nextX_2, nextY_2, snake_2, 2);
    }

    insertField(pixels);
    insertElements(pixels);
    insertScore(pixels);
    return true;
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
    else if (snakeNumber == 2)
        switch (direction_2)
        {
            case 5:
                return snake_2_h_north_pixels_map;
            case 6:
                return snake_2_h_south_pixels_map;
            case 7:
                return snake_2_h_west_pixels_map;
            case 8:
                return snake_2_h_east_pixels_map;
            default:
                return snake_2_h_north_pixels_map;
        }
    return snake_h_north_pixels_map;
}

bool    GameCore::checkTarget(int x, int y, Block* target)
{
    if (target->x == x && target->y == y)
    {
        sound->soundEat();
        scoreCount++;
        increaseSpeed = true;
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
    for (std::uint32_t i = 1; i < snake.size(); ++i)
        if(snake[0]->x == snake[i]->x && snake[0]->y == snake[i]->y)
            return true;
    // Check for a collision of a snake with wall
    if (x < 0 || x > m_width - BLOCK_SIZE || y < 0 || y > m_height - BLOCK_SIZE)
        return true;

    return false;
}

void    GameCore::getLib(int libNumber)
{
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

    disp = create_wrapper(m_width, m_height + scoreBlockHeight);
}

bool    GameCore::getDirection(std::uint8_t *pixels)
{
    int event = disp->getEvent();

    if (event >= 1 && event <= 4)
        direction_1 = event;
    else if (event >= 5 && event <= 8)
        direction_2 = event;
    else if (event >= 10 && event <= 30 && event != currentLib)
    {
        std::function<void(GUIDisplay*)> release_wrapper(reinterpret_cast<void(*)(GUIDisplay*)>(dlsym(lib_discr, "release_wrapper")));
        release_wrapper(disp);
        lib_discr = nullptr;
        currentLib = event;
        getLib(event);
        disp->render(pixels);
    }
    else if (event == 0)
        return false;

    return true;
}

void    GameCore::checkDirection()
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

void    GameCore::showOpening(std::uint8_t *pixels)
{
    insertField(pixels);
    for (int x = 0; x < m_width; x += BLOCK_SIZE)
    {
        insertBlockToScene(x, m_height, 48, 48, fieldPixelMap, pixels);
        insertBlockToScene(x, m_height + scoreBlockHeight / 2, 48, 48, fieldPixelMap, pixels);
    }

    insertBlockToScene((m_width - 320) / 2,
                       (m_height + scoreBlockHeight - 320) / 2,
                       320, 320, startGamePixelMap, pixels);

    sound->startGame();
    disp->render(pixels);
    disp->getEvent();
    showingLoop(3.0f);
}

void    GameCore::gameOver(std::uint8_t *pixels)
{
    fillBackground(pixels, 0, m_width, 0, m_height + scoreBlockHeight, 0);
    insertBlockToScene((m_width - 480) / 2,
                       (m_height + scoreBlockHeight - 480) / 2,
                       480, 480, endGamePixelMap, pixels);
    disp->render(pixels);
    showingLoop(6.0f);
}

void    GameCore::showingLoop(float duration)
{
    Timer timer(duration);
    timer.reset();
    while (true)
    {
        timer.tick();
        if (timer.deltaTime() >= timer.getTimeScale())
            break;
    }
}

void    GameCore::run()
{
    Timer           timer(.5f);
    Timer           bonusTargetTimer(15.0f);
    unsigned int    imageSize = m_width * m_height * scoreBlockWidth * scoreBlockHeight * 4;
    std::uint8_t    *pixels = new std::uint8_t[imageSize];
    bool            isGameOver = false;

    getLib(20);

    showOpening(pixels);

    timer.reset();
    while (direction_1)
    {
        timer.tick();
        bonusTargetTimer.tick();
        if (timer.deltaTime() >= timer.getTimeScale())
        {
            checkDirection();
            timer.reset();
            if (getImage(pixels))
                disp->render(pixels);
            else
            {
                isGameOver = true;
                break;
            }
        }

        if (!getDirection(pixels))
            break;

        if (bonusTargetTimer.deltaTime() >= bonusTargetTimer.getTimeScale())
        {
            bonusTargetTimer.reset();
            bonusTarget->isVisible = !bonusTarget->isVisible;
            updateTarget(bonusTarget);
        }

        if (increaseSpeed && !(scoreCount % 3))
        {
            float timescale = timer.getTimeScale();
            timer.setTimeScale(timescale > 0.2f ? timescale - 0.1f : timescale);
            increaseSpeed = false;
        }

    }

    if (isGameOver)
        gameOver(pixels);

    std::function<void(GUIDisplay*)> release_wrapper(reinterpret_cast<void(*)(GUIDisplay*)>(dlsym(lib_discr, "release_wrapper")));
    release_wrapper(disp);

    delete[] pixels;
}

void *GameCore::loadLib(std::string libname)
{
    void *lib_discriptor;

    lib_discriptor = dlopen(libname.data(), RTLD_LAZY);

    if (!lib_discriptor)
    {
        std::string mess("*** DINAMYC LIBRARY LOADING ERROR ***\ndescriptor is not present: ");
        mess.append(dlerror());
        throw std::invalid_argument(mess);
    }

    return lib_discriptor;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Debug functions                                                                              //
//////////////////////////////////////////////////////////////////////////////////////////////////

int GameCore::pixToInt(int x, int y, int rowWidth, std::uint8_t *pixels)
{
    int idx = (y * rowWidth + x) * 4;
    return ((pixels[idx] << 24) +
            (pixels[idx + 1] << 16) +
            (pixels[idx + 2] << 8) +
            (pixels[idx + 3]));
}
