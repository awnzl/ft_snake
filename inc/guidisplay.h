#ifndef __GUIDISPLAY_H
# define __GUIDISPLAY_H

#include <cstdint>

class GUIDisplay {
public:
    virtual void render(uint8_t *rawImage) = 0;
    virtual int getEvent() = 0;
    virtual ~GUIDisplay() = default;
};


#endif