#include "imgloader.h"
#include <SFML/Graphics.hpp>

uint8_t *IMGLoader::getPixelMap(std::string s)
{
    sf::Image im;
    if (!im.loadFromFile(s))
    {
        std::cerr << "error: imgloader.cpp:16\n";
        exit (1);
    }

    sf::Vector2u dimensions = im.getSize();
    uint32_t size = dimensions.x * dimensions.y * 4;
    uint8_t *ret = new uint8_t[size];

    const uint8_t *pixMap = static_cast<const uint8_t*>(im.getPixelsPtr());

    for (int idx = 0; idx < size; idx++)
        ret[idx] = pixMap[idx];

    return ret;
}

extern "C" IMGLoader *createImgLoader()
{
    return new IMGLoader();
}

extern "C" void releaseImgLoader(IMGLoader *il)
{
    delete il;
}