#include "sfmlwrapper.h"
#include <unistd.h>

//TODO: implement
GUIDisplay::~GUIDisplay()
{
    delete (im);
    delete (texture);
    delete (sprite);
    delete (event);
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

void GUIDisplay::render(uint8_t *rawImage)
{
    im->create(WIDTH_HEIGTH, WIDTH_HEIGTH, rawImage);

    // std::cout << "SFML DEBUG: pixel.toInteger: " << im->getPixel(5,5).toInteger() << std::endl;

    texture->loadFromImage(*im);
    sprite->setTexture(*texture);

    //TODO: this color affect pixels colors - how?
    win->clear(sf::Color::White);
    // texture->update(rawImage);//do I need it ?
    win->draw(*sprite);
    win->display();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// returns an integer wich represent an event                                                   //
//////////////////////////////////////////////////////////////////////////////////////////////////
int GUIDisplay::getEvent()
{
    static int lastDirection = 3;
    while (win->pollEvent(*event)) {
        if (event->type == sf::Event::Closed) {
            win->close();
            return 0;
        } else if (event->key.code == sf::Keyboard::Escape) {
            win->close();
            return 0;
        } else if (event->key.code ==  sf::Keyboard::Left) {
            lastDirection = 2;
        } else if (event->key.code ==  sf::Keyboard::Right) {
            lastDirection = 3;
        } else if (event->key.code ==  sf::Keyboard::Up) {
            lastDirection = 1;
        } else if (event->key.code ==  sf::Keyboard::Down) {
            lastDirection = 4;
        }
    }
	return lastDirection;
}