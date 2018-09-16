#ifndef __AUDIOWRAPPER_H
# define __AUDIOWRAPPER_H

#include <iostream>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL.h>

class AudioWrapper
{
    AudioWrapper(const AudioWrapper &) = delete;
    AudioWrapper(AudioWrapper &&) = delete;
    AudioWrapper &operator=(const AudioWrapper &) = delete;
    AudioWrapper &operator=(AudioWrapper &&) = delete;

    Mix_Chunk *startSound = NULL;
    Mix_Chunk *eat = NULL;
    Mix_Chunk *step = NULL;
    Mix_Music *endSound = NULL;

public:
    AudioWrapper();
    virtual ~AudioWrapper();

    virtual void    startGame();
    virtual void    soundEat();
    virtual void    soundStep();
    virtual void    endGame();
};

#endif