#ifndef __AUDIOWRAPPERIMPL_H
# define __AUDIOWRAPPERIMPL_H

#include <iostream>
#include <AL/alut.h>
#include "audiowrapper.h"


class AudioWrapperImpl : public AudioWrapper
{
    AudioWrapperImpl(const AudioWrapperImpl &) = delete;
    AudioWrapperImpl(AudioWrapperImpl &&) = delete;
    AudioWrapperImpl &operator=(const AudioWrapperImpl &) = delete;
    AudioWrapperImpl &operator=(AudioWrapperImpl &&) = delete;

    ALuint startSound;
    ALuint eat;
    ALuint step;
    ALuint endSound;

    ALuint sources[4];

public:
    AudioWrapperImpl();
    virtual ~AudioWrapperImpl();

    virtual void    startGame();
    virtual void    soundEat();
    virtual void    soundStep();
    virtual void    endGame();
};

#endif