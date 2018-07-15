
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
public:
    GUIDisplay();
    GUIDisplay(const GUIDisplay &);
    virtual ~GUIDisplay();
    GUIDisplay &operator=(GUIDisplay &);

    void render(uint8_t *rawImage);
    int getEvent();
};

#endif