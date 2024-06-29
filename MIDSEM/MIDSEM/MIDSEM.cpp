//3032720
//KRAPAH BLESS ELIKEM
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

const int SRC_WIDTH = 800;
const int SRC_HEIGHT = 600;

//functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void closeOnInput(GLFWwindow* window);
GLuint moveSquare(GLFWwindow* window);
//GLuint resizeSquare(GLFWwindow* window);

const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 Position;\n"
"uniform mat4 transform;"
"void main()\n"
"{\n"
"   gl_Position = transform * vec4(Position, 1.0);\n"
"}\0";

const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "Square Manipulation", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    GLuint vertexShader, fragmentShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    GLfloat vertexData[] = {

        -1.00f, 0.00f, 0.0f,
        -0.75f, 0.25f, 0.0f,
        -0.75f, 0.00f, 0.0f,

        -1.00f, 0.00f, 0.0f,
        -0.75f, 0.25f, 0.0f,
        -1.00f, 0.25f, 0.0f,
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER , VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    GLuint Move = moveSquare(window);
         
    glUseProgram(shaderProgram);
    GLuint transformLoc = glGetUniformLocation(shaderProgram, "transform");
    glm::mat4 trans = glm::mat4(1.0f);
    GLfloat currentFrame, deltaTime;
    float  lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        closeOnInput(window);
        Move = moveSquare(window);
        
        if (Move == 1) {
            trans = glm::translate(trans, glm::vec3(0.0f, 0.5f, 0.0f) * deltaTime);
        }
        else if (Move == 2) {
            trans = glm::translate(trans, glm::vec3(-0.5f, 0.0f, 0.0f) * deltaTime);
        }
        else if (Move == 3) {
            trans = glm::translate(trans, glm::vec3(0.0f, -0.5f, 0.0f) * deltaTime);
        }
        else if (Move == 4) {
            trans = glm::translate(trans, glm::vec3(0.5f, 0.0f, 0.0f) * deltaTime);
        }
        else if (Move == 5) {
            trans = glm::scale(trans, glm::vec3(0.05f, 0.05f, 0.05f) * deltaTime);
        }
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

       
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 2 * 3);
        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void closeOnInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

GLuint moveSquare(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        return 1;
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        return 2;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        return 3;
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        return 4;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
        return 5;
    }
    else {
        return 0;
    }
};

//GLuint resizeSquare(GLFWwindow* window) {
    