#include "sfmlwrapper.h"
#include <unistd.h>

//TODO: implement
GUIDisplay::~GUIDisplay() {
    delete (win);
}

GUIDisplay::GUIDisplay() :
    win(new sf::RenderWindow(sf::VideoMode(WIDTH_HEIGTH, WIDTH_HEIGTH), "nibbler")),
    im(new sf::Image()),
    texture(new sf::Texture),
    sprite(new sf::Sprite()),
    event(new sf::Event())
{
    texture->create(WIDTH_HEIGTH, WIDTH_HEIGTH);
}

//TODO: implement
GUIDisplay::GUIDisplay(const GUIDisplay &gd) {
    this->win = gd.win;
}

//TODO: implement
GUIDisplay &GUIDisplay::operator=(GUIDisplay &gd) {
    this->win = gd.win;
    return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// returns an integer wich represent an event                                                   //
//////////////////////////////////////////////////////////////////////////////////////////////////
int GUIDisplay::run(uint8_t *rawImage) {
    im->create(WIDTH_HEIGTH, WIDTH_HEIGTH, rawImage);

    // std::cout << "SFML DEBUG: pixel.toInteger: " << im->getPixel(5,5).toInteger() << std::endl;

    texture->loadFromImage(*im);
    sprite->setTexture(*texture);
    while (win->pollEvent(*event)) {
        if (event->type == sf::Event::Closed) {
            win->close();
            return -1;
        } else if (event->key.code == sf::Keyboard::Escape) {
            win->close();
            return -1;
        } else if (event->key.code ==  sf::Keyboard::Left) {
            return 2;
        } else if (event->key.code ==  sf::Keyboard::Right) {
            return 3;
        } else if (event->key.code ==  sf::Keyboard::Up) {
            return 1;
        } else if (event->key.code ==  sf::Keyboard::Down) {
            return 4;
        }
    }
    //TODO: this color affect pixels colors - how?
    win->clear(sf::Color::White);
    // texture->update(rawImage);//do I need it ?
    win->draw(*sprite);
    win->display();

	return 0;
}