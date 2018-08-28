#include "sfmlwrapper.h"

//TODO: implement
SFMLWrapper::~SFMLWrapper()
{
    delete (im);
    delete (texture);
    delete (sprite);
    delete (event);
    delete (win);
}

SFMLWrapper::SFMLWrapper(int w, int h) :
    win(new sf::RenderWindow(sf::VideoMode(w, h), "nibbler")),
    im(new sf::Image()),
    texture(new sf::Texture),
    sprite(new sf::Sprite()),
    event(new sf::Event())
{
    m_width = w;
    m_height = h;
    texture->create(m_width, m_height);
}

void SFMLWrapper::render(uint8_t *rawImage)
{
    im->create(m_width, m_height, rawImage);

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
int SFMLWrapper::getEvent()
{
    static int lastDirection = 3;
    while (win->pollEvent(*event)) {
        if (event->type == sf::Event::Closed ||
            event->key.code == sf::Keyboard::Escape) {
            win->close();
            return 0;
        } else if (event->key.code ==  sf::Keyboard::Left) {
            lastDirection = 2;
            break;
        } else if (event->key.code ==  sf::Keyboard::Right) {
            lastDirection = 3;
            break;
        } else if (event->key.code ==  sf::Keyboard::Up) {
            lastDirection = 1;
            break;
        } else if (event->key.code ==  sf::Keyboard::Down) {
            lastDirection = 4;
            break;
        }
    }
	return lastDirection;
}

extern "C" GUIDisplay *create_wrapper(int w, int h)
{
    return new SFMLWrapper(w, h);
}

extern "C" void release_wrapper(GUIDisplay *wrapper)
{
    delete wrapper;
}