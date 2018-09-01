#include "glfwwrapper.h"

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
int         lastDirection;

GLFWWrapper::~GLFWWrapper()
{
    glfwTerminate();
}

GLFWWrapper::GLFWWrapper(int width, int height)
{
    m_width = width;
    m_height = height;
    lastDirection = -1;
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    win = glfwCreateWindow(m_width / 2, m_height / 2, "Nibbler", NULL, NULL);
    if(!win)
    {
        std::cout << "HER\n";
        exit(10);
    }
    glfwMakeContextCurrent(win);
    glfwSetKeyCallback(win, key_callback);
}

void GLFWWrapper::render(uint8_t *rawImage)
{
    std::cout << "GLFWWrapper::render 1" << std::endl;
    // Clear the colorbuffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glRasterPos2f(-1,1);
    glPixelZoom( 1, -1 );
    glDrawPixels(m_width, m_height, GL_RGBA,
        GL_UNSIGNED_INT_8_8_8_8_REV, rawImage);
    // Swap the screen buffers
    glfwSwapBuffers(win);
    std::cout << "GLFWWrapper::render 2" << std::endl;
}

int GLFWWrapper::getEvent()
{
    std::cout << "GLFWWrapper::getEvent 1" << std::endl;
    // Check if any events have been activiated (key pressed, mouse moved etc.)
    // and call corresponding response functions
    if (!win)
        std::cout << "win not good" << std::endl;
    glfwPollEvents();
    
    if(glfwWindowShouldClose(win))
        return 0;
    std::cout << lastDirection << std::endl;
    std::cout << "GLFWWrapper::getEvent 2" << std::endl;
	return lastDirection;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
        lastDirection = 0;
    }
    else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        lastDirection = 2;
    else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        lastDirection = 3;
    else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        lastDirection = 1;
    else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        lastDirection = 4;
    else if (key == GLFW_KEY_1)
        lastDirection = 10;
    else if (key == GLFW_KEY_2)
        lastDirection = 20;
    else if (key == GLFW_KEY_3)
        lastDirection = 30;
}

extern "C" GUIDisplay *create_wrapper(int w, int h)
{
    return new GLFWWrapper(w, h);
}

extern "C" void release_wrapper(GUIDisplay *wrapper)
{
    delete wrapper;
}