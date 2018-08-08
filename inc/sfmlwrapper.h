#ifndef __SFMLWRAPPER_H
# define __SFMLWRAPPER_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "defines.h"
#include "guidisplay.h"

class SfmlWrapper : public GUIDisplay {
    sf::RenderWindow *win;

    sf::Image *im;
    sf::Texture *texture;
    sf::Sprite *sprite;
    sf::Event *event;

    SfmlWrapper(SfmlWrapper const &) = delete;
    SfmlWrapper(SfmlWrapper &&) = delete;
    SfmlWrapper &operator=(SfmlWrapper &) = delete;
    SfmlWrapper &operator=(SfmlWrapper &&) = delete;

public:
    SfmlWrapper();
    ~SfmlWrapper();

    void render(uint8_t *rawImage) override;
    int getEvent() override;
};

#endif