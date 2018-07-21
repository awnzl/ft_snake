#include "timer.h"

Timer::Timer()
{
    _timeScale = 1.0f;
    _deltaTime = std::chrono::duration<float>(0.0f);
}

void Timer::reset()
{
    _startTime = std::chrono::system_clock::now();
}

float Timer::deltaTime()
{
    return _deltaTime.count();
}

void Timer::setTimeScale(float t /* = 1.0f */)
{
    _timeScale = t;
}

float Timer::getTimeScale()
{
    return _timeScale;
}

void Timer::tick()
{
    _deltaTime = std::chrono::system_clock::now() - _startTime;
}