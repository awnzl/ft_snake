#include "glfwwrapper.h"

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
static int lastDirection = 3;

GlfwWrapper::~GlfwWrapper()
{
    glfwTerminate();
}

GlfwWrapper::GlfwWrapper()
{
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    win = glfwCreateWindow(WIDTH_HEIGTH / 2, WIDTH_HEIGTH / 2, "Nibbler", NULL, NULL);
    glfwMakeContextCurrent(win);
    glfwSetKeyCallback(win, key_callback);
}

void GlfwWrapper::render(uint8_t *rawImage)
{
    // Clear the colorbuffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glRasterPos2f(-1,1);
    glPixelZoom( 1, -1 );
    glDrawPixels(WIDTH_HEIGTH, WIDTH_HEIGTH, GL_RGBA,
        GL_UNSIGNED_INT_8_8_8_8_REV, rawImage);
    // Swap the screen buffers
    glfwSwapBuffers(win);
}

int GlfwWrapper::getEvent()
{
    // Check if any events have been activiated (key pressed, mouse moved etc.)
    // and call corresponding response functions
    glfwPollEvents();
    if(glfwWindowShouldClose(win))
        return 0;
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
}