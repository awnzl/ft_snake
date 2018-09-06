#include "sfmlwrapper.h"

SFMLWrapper::~SFMLWrapper()
{
    delete (sprite);
    delete (texture);
    delete (im);
    delete (win);
    delete (event);
}

SFMLWrapper::SFMLWrapper(int w, int h) :
    m_width(w),
    m_height(h),
    win(new sf::RenderWindow(sf::VideoMode(w, h), "Nibbler/SFML")),
    im(new sf::Image()),
    texture(new sf::Texture),
    sprite(new sf::Sprite),
    event(new sf::Event)
{
    lastDirection = -1;
    texture->create(m_width, m_height);
}

void SFMLWrapper::render(uint8_t *rawImage)
{
    im->create(m_width, m_height, rawImage);

    texture->loadFromImage(*im);
    sprite->setTexture(*texture);
    win->clear(sf::Color::White);
    win->draw(*sprite);
    win->display();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// returns an integer wich represent an event                                                   //
//////////////////////////////////////////////////////////////////////////////////////////////////
int SFMLWrapper::getEvent()
{
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
        } else if (event->key.code ==  sf::Keyboard::W) {
            lastDirection = 5;
            break;
        } else if (event->key.code ==  sf::Keyboard::S) {
            lastDirection = 6;
            break;
        } else if (event->key.code ==  sf::Keyboard::A) {
            lastDirection = 7;
            break;
        } else if (event->key.code ==  sf::Keyboard::D) {
            lastDirection = 8;
            break;
        } else if (event->key.code ==  sf::Keyboard::Num1) {
            lastDirection = 10;
            break;
        } else if (event->key.code ==  sf::Keyboard::Num2) {
            lastDirection = 20;
            break;
        } else if (event->key.code ==  sf::Keyboard::Num3) {
            lastDirection = 30;
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