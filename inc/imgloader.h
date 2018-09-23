#ifndef __IMGLOADER_HPP
# define __IMGLOADER_HPP

#include <cstdint>
#include <string>

class IMGLoader
{
public:
    virtual ~IMGLoader() = default;

    virtual uint8_t *getPixelMap(std::string s) = 0;
};

#endif