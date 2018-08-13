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
    win = glfwCreateWindow(WIDTH_HEIGTH / 2, WIDTH_HEIGTH / 2, "Hello World", NULL, NULL);
    glfwMakeContextCurrent(win);
    glfwSetKeyCallback(win, key_callback);
    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(win, &width, &height);  
    glViewport(0, 0, width, height);
    glMatrixMode( GL_PROJECTION ); // projection matrix defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
    glLoadIdentity( ); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
    glOrtho( 0, WIDTH_HEIGTH, 0, WIDTH_HEIGTH, 0, 1 ); // essentially set coordinate system
    glMatrixMode( GL_MODELVIEW ); // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world
    glLoadIdentity( ); // same as above comment
}

void GlfwWrapper::render(uint8_t *rawImage)
{
    // Clear the colorbuffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    GLfloat polygonVertices[] =
    {
        20, 100, 0,
        100, 300, 0,
        500, 50, 0,
        320, 10, 0,
        40, 40, 0
    };
    glPolygonMode( GL_FRONT_AND_BACK, GL_POINT ); // polygon drawing mode (GL_POINT, GL_LINE, GL_FILL)
    printPolygon(polygonVertices);
    // Swap the screen buffers
    glfwSwapBuffers(win);
}

void GlfwWrapper::printPolygon(GLfloat pointVertex[])
{
    // Render OpenGL here
    glEnable( GL_POINT_SMOOTH ); // make the point circular
    glEnableClientState( GL_VERTEX_ARRAY ); // tell OpenGL that you're using a vertex array for fixed-function attribute
    glPointSize( 50 ); // must be added before glDrawArrays is called
    glVertexPointer( 3, GL_FLOAT, 0, pointVertex ); // point to the vertices to be used
    glDrawArrays( GL_POLYGON, 0, 5 ); // draw the vertixes
    glDisableClientState( GL_VERTEX_ARRAY ); // tell OpenGL that you're finished using the vertex arrayattribute
    glDisable( GL_POINT_SMOOTH ); // stop the smoothing to make the points circular
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