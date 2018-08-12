#include "sdl2wrapper.h"

SDL2Wrapper::SDL2Wrapper()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    if (SDL_CreateWindowAndRenderer(WIDTH_HEIGTH / 2,
                                    WIDTH_HEIGTH / 2, SDL_WINDOW_SHOWN, &win, &ren))
    {
        std::cerr << "SDL_CreateWindowAndRenderer Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    if (!win || !ren)
    {
        std::cerr << "SDL_CreateWindowAndRenderer Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    // checkError((!win || !ren), "SDL_CreateWindowAndRenderer Error: ");

    //https://wiki.libsdl.org/SDL_PixelFormatEnum
    tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ABGR8888,
                            SDL_TEXTUREACCESS_TARGET, WIDTH_HEIGTH, WIDTH_HEIGTH);

    if (!tex)
    {
        std::cerr << "SDL_CreateTexture Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    // checkError(!tex, "SDL_CreateTexture Error: ");

    event = new SDL_Event();
}

SDL2Wrapper::~SDL2Wrapper()
{
    delete event;
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

void SDL2Wrapper::checkError(bool condition, std::string s)
{
    if (condition)
    {
        std::cerr << s << SDL_GetError() << std::endl;
        exit (1);
    }
}

void SDL2Wrapper::render(uint8_t *rawImage)
{
    SDL_UpdateTexture(tex, NULL, rawImage, WIDTH_HEIGTH * sizeof(uint32_t));

    std::cerr << "debug:/n";

    SDL_RenderClear(ren);
    SDL_RenderCopy(ren, tex, NULL, NULL);
    SDL_RenderPresent(ren);
}

int SDL2Wrapper::getEvent()
{
    static int lastDirection = 3;

    SDL_PollEvent(event);

    switch (event->type) {
        case SDL_QUIT:
            return 0;// TODO: replayce by relese function?
        case SDL_KEYDOWN:
            switch (event->key.keysym.sym) {
                case SDLK_ESCAPE:
                    return 0;
                case SDLK_LEFT:
                    lastDirection = 2;
                    break;
                case SDLK_RIGHT:
                    lastDirection = 3;
                    break;
                case SDLK_UP:
                    lastDirection = 1;
                    break;
                case SDLK_DOWN:
                    lastDirection = 4;
                    break;
                default:
                    break;
            }
    }

	return lastDirection;
}