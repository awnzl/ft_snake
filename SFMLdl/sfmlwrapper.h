#ifndef __SFMLWRAPPER_H
# define __SFMLWRAPPER_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "defines.h"
#include "guidisplay.h"

class SFMLWrapper : public GUIDisplay
{
    int                 m_width;
    int                 m_height;
    int                 lastDirection;
    sf::RenderWindow    *win;
    sf::Image           *im;
    sf::Texture         *texture;
    sf::Sprite          *sprite;
    sf::Event           *event;

    SFMLWrapper() = delete;
    SFMLWrapper(SFMLWrapper const &) = delete;
    SFMLWrapper(SFMLWrapper &&) = delete;
    SFMLWrapper &operator=(SFMLWrapper &) = delete;
    SFMLWrapper &operator=(SFMLWrapper &&) = delete;

public:
    SFMLWrapper(int w, int h);
    ~SFMLWrapper();

    void    render(uint8_t *rawImage) override;
    int     getEvent() override;
};

#endif