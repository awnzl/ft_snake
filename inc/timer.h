#ifndef __TIMER_H
# define __TIMER_H

#include <chrono>

class Timer
{
    Timer(Timer const &) = delete;
    Timer(Timer &&) = delete;
    Timer &operator=(const Timer &) = delete;
    Timer &operator=(Timer &&) = delete;

    std::chrono::system_clock::time_point _startTime;
    std::chrono::duration<float> _deltaTime;
    float _timeScale;

public:
    Timer();
    ~Timer() = default;

    void reset();
    float deltaTime();
    void setTimeScale(float t = 1.0f);
    float getTimeScale();
    void tick();
};

#endif