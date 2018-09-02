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
    win(new sf::RenderWindow(sf::VideoMode(w, h), "nibbler")),
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
    // std::cout << "SFMLWrapper::render 1" << std::endl;
    im->create(m_width, m_height, rawImage);

    texture->loadFromImage(*im);
    sprite->setTexture(*texture);
    win->clear(sf::Color::White);
    win->draw(*sprite);
    win->display();
    // std::cout << "SFMLWrapper::render 2" << std::endl;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// returns an integer wich represent an event                                                   //
//////////////////////////////////////////////////////////////////////////////////////////////////
int SFMLWrapper::getEvent()
{
    // std::cout << "SFMLWrapper::getEvent 1" << std::endl;
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
    // std::cout << lastDirection << std::endl;
    // std::cout << "SFMLWrapper::getEvent 2" << std::endl;
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