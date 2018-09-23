#include "sdl2wrapper.h"

SDL2Wrapper::SDL2Wrapper(int w, int h)
{
    m_width = w;
    m_height = h;
    lastDirection = -1;

    checkError(SDL_Init(SDL_INIT_VIDEO) != 0, "SDL_Init Error: ");

    checkError((-1 == SDL_CreateWindowAndRenderer(m_width / 2,
                m_height / 2, SDL_WINDOW_SHOWN, &win, &ren)),
        "SDL_CreateWindowAndRenderer Error: ");

    SDL_SetWindowTitle(win, "Nibbler/SDL2");

    tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ABGR8888,
                            SDL_TEXTUREACCESS_TARGET, m_width, m_height);

    checkError(!tex, "SDL_CreateTexture Error: ");

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
    SDL_UpdateTexture(tex, NULL, rawImage, m_width * sizeof(uint32_t));

    SDL_RenderClear(ren);
    SDL_RenderCopy(ren, tex, NULL, NULL);
    SDL_RenderPresent(ren);
}

int SDL2Wrapper::getEvent()
{

    SDL_PollEvent(event);

    switch (event->type) {
        case SDL_QUIT:
            return 0;
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
                case SDLK_w:
                    lastDirection = 5;
                    break;
                case SDLK_s:
                    lastDirection = 6;
                    break;
                case SDLK_a:
                    lastDirection = 7;
                    break;
                case SDLK_d:
                    lastDirection = 8;
                    break;
                case SDLK_1:
                    lastDirection = 10;
                    break;
                case SDLK_2:
                    lastDirection = 20;
                    break;
                case SDLK_3:
                    lastDirection = 30;
                    break;
                default:
                    break;
            }
    }

	return lastDirection;
}

extern "C" GUIDisplay *create_wrapper(int w, int h)
{
    return new SDL2Wrapper(w, h);
}

extern "C" void release_wrapper(GUIDisplay *wrapper)
{
    delete wrapper;
}