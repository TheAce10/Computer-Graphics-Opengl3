#ifndef SHADER_H
#define SHADER_H
#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

//shader header file
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


#include <iostream>


//FUNCTION IDENTIFIER
void processInput(GLFWwindow* );
void frameBufferSizeCallback(GLFWwindow*, int, int );

//WINDOW SETTINGS
const unsigned int height = 600;
const unsigned int width = 800;
//GLfloat color[] = {1, 1, 1};

class Shader
{
public:
    // the program ID
    unsigned int ID;
    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file’s buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        // 2. compile shaders
        unsigned int vertex, fragment;
        int success;
        char infoLog[512];
        // vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        // print compile errors if any
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
                infoLog << std::endl;
        };
        // similar for Fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        // print compile errors if any
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
                infoLog << std::endl;
        };
        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        // print linking errors if any
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
                infoLog << std::endl;
        }
        // delete shaders; they’re linked into our program and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    };
    // use/activate the shader
        void use()
        {
            glUseProgram(ID);
        }

    // utility uniform functions
        void setBool(const std::string& name, bool value) const
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
        }
        void setInt(const std::string& name, int value) const
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }
        void setFloat(const std::string& name, float value) const
        {
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
        }
};

/*//SHADERS
const char* vertShaderSource =
"#version 330 core\n"
"layout(location = 0) in vec3 Pos;\n"
"layout(location = 1) in vec2 TexCoord;\n"
"layout(location = 2) in vec3 Col;\n"
"out vec2 sTexCoord;\n"
"out vec3 sCol;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(Pos, 1.00f);\n"
"   sTexCoord = TexCoord;\n"
"   sCol = Col;"
"}\0";

const char* fragShaderSource =
"#version 330 core\n"
"out vec4 fCol;\n"
"in vec2 sTexCoord;\n"
"in vec3 sCol;\n"
"uniform sampler2D fTexSampler;"

"void main()\n"
"{\n"
"   fCol = texture(fTexSampler, sTexCoord) * vec4(sCol, 0.0);\n"
"}\0";

*/

#endif


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Project", NULL, NULL);

    if (window == NULL) {
        std::cout << "window failed to create" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "failed to initialize glad" << std::endl;
        return -1  ;
    }
    /*
    //SHADER CODE
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vertShaderSource, NULL);
    glCompileShader(vertShader);

    int success;
    char infolog[512];
    //compilation success check
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertShader, 512, NULL, infolog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog << std::endl;
    }

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragShaderSource, NULL);
    glCompileShader(fragShader);
    //compilation success check
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragShader, 512, NULL, infolog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infolog << std::endl;
    }

    //LINK SHADERS WITH PROGRAM
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infolog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infolog << std::endl;
    }
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    */
    Shader ourShader = Shader("C:/Users/ACE/Documents/COE 4/SEM 1/COMPUTER GRAPHICS/LABS/SHADERS/vshader.txt", "C:/Users/ACE/Documents/COE 4/SEM 1/COMPUTER GRAPHICS/LABS/SHADERS/fshader.txt");

    glm::vec4 vec1(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 transf = glm::mat4(1.0f);
    transf = glm::translate(transf, glm::vec3(0.0f, 1.0f, 0.0f));
    vec1 = transf * vec1;
    std::cout << vec1.x << vec1.y << vec1.z << std::endl;

    transf = glm::rotate(transf, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    transf = glm::scale(transf, glm::vec3(0.5, 0.5, 0.5));

    

    //Everything Starts Here
    const GLfloat coords[] = 
    {
       -0.225f, 0.700f, 0.00f, 0.00f, 1.00f, 0.50f, 0.50f, 0.50f,
       -0.225f, 0.000f, 0.00f, 0.00f, 0.00f, 0.50f, 0.50f, 0.50f,
        0.475f, 0.000f, 0.00f, 1.00f, 0.00f, 0.50f, 0.50f, 0.50f,

       -0.225f, 0.700f, 0.00f, 0.00f, 1.00f, 0.50f, 0.50f, 0.50f,
        0.475f, 0.000f, 0.00f, 1.00f, 0.00f, 0.50f, 0.50f, 0.50f,
        0.475f, 0.700f, 0.00f, 1.00f, 1.00f, 0.50f, 0.50f, 0.50f,

        0.000f, 0.000f, 0.00f, 0.00f, 0.00f, 0.35f, 1.0f, 1.0f,
        0.250f, 0.000f, 0.00f, 0.00f, 0.00f, 0.35f, 1.0f, 1.0f,
        0.125f, 0.250f, 0.00f, 0.00f, 0.00f, 0.35f, 1.0f, 1.0f,

        0.000f, 0.700f, 0.00f, 0.00f, 0.00f, 0.35f, 1.0f, 1.0f,
        0.250f, 0.700f, 0.00f, 0.00f, 0.00f, 0.35f, 1.0f, 1.0f,
        0.125f, 0.450f, 0.00f, 0.00f, 0.00f, 0.35f, 1.0f, 1.0f,

       -0.225f, 0.225f, 0.00f, 0.00f, 0.00f, 0.35f, 1.0f, 1.0f,
       -0.225f, 0.475f, 0.00f, 0.00f, 0.00f, 0.35f, 1.0f, 1.0f,
        0.025f, 0.350f, 0.00f, 0.00f, 0.00f, 0.35f, 1.0f, 1.0f,

        0.475f, 0.225f, 0.00f, 0.00f, 0.00f, 0.00f, 1.0f, 1.0f,
        0.475f, 0.475f, 0.00f, 0.00f, 0.00f, 0.00f, 1.0f, 1.0f,
        0.225f, 0.350f, 0.00f, 0.00f, 0.00f, 0.00f, 1.0f, 1.0f,
        
    };

    GLuint VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER ,VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);

    unsigned int texture, texture1, texture2;
    glGenTextures(1, &texture);
    //glGenTextures(1, &texture1);
    //glGenTextures(1, &texture2);
    //glBindTexture(GL_TEXTURE_2D, texture);
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, texture1);
    //glActiveTexture(GL_TEXTURE0+1);
    //glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping/filtering options (on currently bound texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int imgheight, imgwidth, nrChannels;
    unsigned char* data = stbi_load("C:/Users/ACE/Documents/COE 4/SEM 1/COMPUTER GRAPHICS/LABS/images/OIP.jpg", &imgwidth, &imgheight, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgwidth, imgheight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);


    //position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);
    // texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // color attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    std::cout << "SIZE IS " << sizeof(coords) << std::endl;

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        //BG COLOR
        glClearColor(0.35f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //execute program
        /*glUseProgram(shaderProgram);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3 * 6);
        */

        ourShader.use();
        //ourShader.setFloat("fTexSampler", 1.0f);
        glDrawArrays(GL_TRIANGLES, 0, 3 * 6);
        GLuint transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transf));
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

//FUNCTION DEFINITION
void frameBufferSizeCallback(GLFWwindow* window, int weight, int height) {
    glViewport(0, 0, weight, height);
}


void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
};

