//#define GLFW_INCLUDE_NONE
//#define GLFW_INCLUDE_GLCOREARB
//#define GLFW_DLL

//#include "glad/glad.h"

#include <iostream>
#include "glfw3.h"
using namespace std;

void processInputs(GLFWwindow*);

int main()
{
    cout << "Hello world!" << endl;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    //gflwCreateWindow(initHeight, initWidth,title, NULL,NULL);
    GLFWwindow* window = glfwCreateWindow(800, 640, "Hello Triangle", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);//render the window else it stays in the buffer

    //where we are rendering at
    //glViewport(0,0,800,640);

    //notice we are not using the glfw command
    //the glfw needs a command to access the graphics card thats OpenGL
    while (!glfwWindowShouldClose(window)) {
        processInputs(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        //front buffer current command
        //back buffer the next command
        glfwPollEvents();
    }

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData()

        return 0;
}

GLFWwindow* window = glfwCreateWindow(800, 640, "Hello Triangle", NULL, NULL);

void processInputs(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}



