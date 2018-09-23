#include "imgloaderimpl.h"
#include <SFML/Graphics.hpp>

void    IMGLoaderImpl::printError(std::string err)
{
    std::cerr << "Image load error:\n\t" << err << std::endl;
    exit(1);
}

uint8_t *IMGLoaderImpl::getPixelMap(std::string s)
{
    if (s.empty())
        printError("empty asset path");
    sf::Image im;
    if (!im.loadFromFile(s))
        printError("wrong asset path?");

    sf::Vector2u dimensions = im.getSize();
    uint32_t size = dimensions.x * dimensions.y * 4;
    uint8_t *ret = new uint8_t[size];

    const uint8_t *pixMap = static_cast<const uint8_t*>(im.getPixelsPtr());

    for (std::uint32_t idx = 0; idx < size; idx++)
        ret[idx] = pixMap[idx];

    return ret;
}

extern "C" IMGLoaderImpl *createImgLoader()
{
    return new IMGLoaderImpl();
}

extern "C" void releaseImgLoader(IMGLoader *il)
{
    delete il;
}