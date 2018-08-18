#ifndef __AUDIOWRAPPER_H
# define __AUDIOWRAPPER_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class AudioWrapper {

    AudioWrapper(AudioWrapper const &) = delete;
    AudioWrapper(AudioWrapper &&) = delete;
    AudioWrapper &operator=(AudioWrapper &) = delete;
    AudioWrapper &operator=(AudioWrapper &&) = delete;

    sf::SoundBuffer buffer;
    sf::Sound sound;

public:
    AudioWrapper();
    ~AudioWrapper();

    void    soundStartGame();
};

#endif