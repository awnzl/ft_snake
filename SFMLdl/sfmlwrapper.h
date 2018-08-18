#ifndef __SFMLWRAPPER_H
# define __SFMLWRAPPER_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "defines.h"
#include "guidisplay.h"

class SFMLWrapper : public GUIDisplay {
    sf::RenderWindow *win;

    sf::Image *im;
    sf::Texture *texture;
    sf::Sprite *sprite;
    sf::Event *event;

    SFMLWrapper(SFMLWrapper const &) = delete;
    SFMLWrapper(SFMLWrapper &&) = delete;
    SFMLWrapper &operator=(SFMLWrapper &) = delete;
    SFMLWrapper &operator=(SFMLWrapper &&) = delete;

public:
    SFMLWrapper();
    ~SFMLWrapper();

    void render(uint8_t *rawImage) override;
    int getEvent() override;
};

#endif