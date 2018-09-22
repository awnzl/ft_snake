#ifndef __AUDIOWRAPPER_H
# define __AUDIOWRAPPER_H

#include <iostream>
#include <AL/alut.h>

class AudioWrapper
{
    AudioWrapper(const AudioWrapper &) = delete;
    AudioWrapper(AudioWrapper &&) = delete;
    AudioWrapper &operator=(const AudioWrapper &) = delete;
    AudioWrapper &operator=(AudioWrapper &&) = delete;

    // data into buffer
    ALuint startSound;
    ALuint eat;
    ALuint step;
    ALuint endSound;

    ALuint sources[4];

public:
    AudioWrapper();
    virtual ~AudioWrapper();

    virtual void    startGame();
    virtual void    soundEat();
    virtual void    soundStep();
    virtual void    endGame();
};

#endif