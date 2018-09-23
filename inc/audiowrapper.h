#ifndef __AUDIOWRAPPER_H
# define __AUDIOWRAPPER_H

class AudioWrapper
{
public:
    virtual ~AudioWrapper() = default;

    virtual void    startGame() = 0;
    virtual void    soundEat() = 0;
    virtual void    soundStep() = 0;
    virtual void    endGame() = 0;
};

#endif