#ifndef __IMGLOADER_HPP
# define __IMGLOADER_HPP

#include <iostream>

class IMGLoader {
    void printError(std::string s);
public:
    IMGLoader()= default;
    ~IMGLoader() = default;

    virtual uint8_t *getPixelMap(std::string s);
};

#endif