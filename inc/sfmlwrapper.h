#ifndef __SFMLWRAPPER_H
# define __SFMLWRAPPER_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "defines.h"

class GUIDisplay {
    sf::RenderWindow *win;

    sf::Image *im;
    sf::Texture *texture;
    sf::Sprite *sprite;
    sf::Event *event;

    GUIDisplay(GUIDisplay const &) = delete;
    GUIDisplay(GUIDisplay &&) = delete;
    GUIDisplay &operator=(GUIDisplay &) = delete;
    GUIDisplay &operator=(GUIDisplay &&) = delete;

public:
    GUIDisplay();
    ~GUIDisplay();

    void render(uint8_t *rawImage);
    int getEvent();
};

#endif