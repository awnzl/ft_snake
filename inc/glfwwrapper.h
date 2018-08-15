#ifndef __GLFWWRAPPER_H
# define __GLFWWRAPPER_H

#include <iostream>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include "defines.h"
#include "guidisplay.h"

class GlfwWrapper : public GUIDisplay {
    GLFWwindow *win;
  
    GlfwWrapper(GlfwWrapper const &) = delete;
    GlfwWrapper(GlfwWrapper &&) = delete;
    GlfwWrapper &operator=(GlfwWrapper &) = delete;
    GlfwWrapper &operator=(GlfwWrapper &&) = delete;

public:
    GlfwWrapper();
    ~GlfwWrapper();

    void render(uint8_t *rawImage) override;
    int getEvent() override;
};

#endif