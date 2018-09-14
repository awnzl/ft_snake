#ifndef __SDL2WRAPPER_H
# define __SDL2WRAPPER_H

#include <iostream>
#include <SDL2/SDL.h>
#include "guidisplay.h"

class SDL2Wrapper : public GUIDisplay
{
    int          m_width;
    int          m_height;
    int          lastDirection;
    SDL_Window   *win;
    SDL_Renderer *ren;
    SDL_Texture  *tex;
    SDL_Event    *event;

    SDL2Wrapper() = delete;
    SDL2Wrapper(SDL2Wrapper const &) = delete;
    SDL2Wrapper(SDL2Wrapper &&) = delete;
    SDL2Wrapper &operator=(SDL2Wrapper &) = delete;
    SDL2Wrapper &operator=(SDL2Wrapper &&) = delete;

    void    checkError(bool, std::string);

public:
    SDL2Wrapper(int w, int h);
    ~SDL2Wrapper();

    void    render(uint8_t *rawImage) override;
    int     getEvent() override;
};

#endif