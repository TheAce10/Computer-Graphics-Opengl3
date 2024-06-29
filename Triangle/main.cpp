#define GLFW_DLL

#include <glad/glad.h>
//#include <GL/gl.h>
#include <GLFW/glfw3.h>
//#include <cstddef>
#include <iostream>


void processInputs(GLFWwindow* window);

float coordinates []= {
        0.5f, 0.5f,
        0.0f , 0.5f,
        0.0f, 0.0f,

         0.0f, 0.0f,
        -0.5f, -0.5f,
         0.0f, -0.5f
        };

class Window{

};

int main(void)
{
    if (glfwInit() == GLFW_FALSE){
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Triangle", NULL, NULL);

    if(window == NULL){
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    gladLoadGL();

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glClearColor(0.2f,0.2f,0.3f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(coordinates),coordinates,GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),0);

    const char *vertex = "#version 330 core\n"
                    "layout (location=0) in vec2 coords;\n"
                    "void main(){\n"
                    "gl_Position = vec4(coords,0.0f,1.0f);\n"
                    "}\0";

    const char *fragment = "#version 330 core\n"
                "out vec4 FragColor;\n"
                "void main(){\n"
                "FragColor = vec4(1.0f,1.5f,0.2f,1.0f);\n"
                "}\0";

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex, NULL);
    glCompileShader(vertex_shader);

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader,1, &fragment, NULL);
    glCompileShader(fragment_shader);

    int success;
    int infoColor(512);

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);


    while(!glfwWindowShouldClose(window)){
        processInputs(window);
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;

}


void processInputs(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
