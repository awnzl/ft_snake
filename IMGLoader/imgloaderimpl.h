#ifndef __IMGLOADERIMPL_HPP
# define __IMGLOADERIMPL_HPP

#include <iostream>
#include <imgloader.h>

class IMGLoaderImpl : public IMGLoader
{
    void printError(std::string s);

    IMGLoaderImpl(const IMGLoaderImpl &) = delete;
    IMGLoaderImpl(IMGLoaderImpl &&) = delete;
    IMGLoaderImpl &operator=(const IMGLoaderImpl &) = delete;
    IMGLoaderImpl &operator=(IMGLoaderImpl &&) = delete;
public:
    IMGLoaderImpl() = default;
    virtual ~IMGLoaderImpl() = default;

    virtual uint8_t *getPixelMap(std::string s);
};

#endif