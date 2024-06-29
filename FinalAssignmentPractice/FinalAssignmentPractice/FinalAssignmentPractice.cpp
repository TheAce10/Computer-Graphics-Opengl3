//3032720
//KRAPAH BLESS ELIKEM
#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include <cmath>


#include <iostream>

const int SRC_WIDTH = 800;
const int SRC_HEIGHT = 600;

//functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void closeOnInput(GLFWwindow* window);

const GLchar* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 Position;\n"
"layout (location = 1) in vec2 oTexCoord;\n"
"out vec2 sTexCoord;\n"
"uniform mat4 transform;"
"uniform mat4 model;"
"uniform mat4 view;"
"uniform mat4 projection;"
"void main()\n"
"{\n"
"   gl_Position = projection * view * model * vec4(Position, 1.0f);\n"
"	sTexCoord = vec2(oTexCoord.x, oTexCoord.y);\n"
"}\0";

const GLchar* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 sTexCoord;\n"
"uniform vec4 Color1;\n"
"uniform sampler2D ourTexture;\n"
"void main()\n"
"{\n"
"   FragColor = texture(ourTexture, sTexCoord) * Color1;\n"
"}\0";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "DVD Animation", NULL, NULL);
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

         0.00f, 0.00f, 0.0f,0.0f, 0.0f,
         0.25f, 0.25f, 0.0f,1.0f, 1.0f,
         0.25f, 0.00f, 0.0f,1.0f, 0.0f,

         0.00f, 0.00f, 0.0f,0.0f, 0.0f,
         0.25f, 0.25f, 0.0f,1.0f, 1.0f,
         0.00f, 0.25f, 0.0f,0.0f, 1.0f,
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // load and create a texture
    // -------------------------
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("C:/Users/ACE/Documents/COE 4/SEM 1/COMPUTER GRAPHICS/LABS/images/dvd-video-logo-png-transparent1.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //glUseProgram(shaderProgram);
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");
    
    GLuint colorLocation = glGetUniformLocation(shaderProgram, "Color1");

    //MVP
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 test = glm::mat4(1.0f);

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f,
        100.0f);

    glm::mat4 view = glm::mat4(1.0f);
    // note that we’re translating the scene in the reverse direction
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));


    glUseProgram(shaderProgram);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    GLfloat currentFrame, deltaTime;
    float  lastFrame = 0.0f;

    float redValue;         //red value to set color
    float greenValue;       //green value to set color
    float blueValue;        //blue value to set color
    int count = 0;          //used to alt between colors. changes when obj hits boundary
    float pi = 3.141593;    //pi value 3.141593 stored here

    int angle= 45;         //state projection angle here, must be integer
        
    //convert angle to range 0 - 359
    if (angle < 0) {        //positive angle
        while (angle < 0)
            angle += 360;
    }
    else if (angle >= 0) { //negative angle
        while (!(angle < 360))
            angle -= 360;
    }
    std::cout << angle<<std::endl; //print angle of projection to screen

    float tval;//tval is the translation value 
    float x, y;//x, y are translation values through  x and y plane respectively
    tval = 0.5f;        //set tval to 

    if (angle <= 45) {//using angle to set translation values
        x = tval;
        y = tan(angle * pi / 180) * tval;
    }
    else if (angle <= 90) {
        y = tval;
        x = tan((90 - angle) * pi / 180) * tval;
    }
    else if (angle <= 135) {
        y = tval;
        x = -tan((angle - 90) * pi / 180) * tval;
    }
    else if (angle <= 180) {
        x = -tval;
        y = tan((180 - angle) * pi / 180) * tval;
    }
    else if (angle <= 225) {
        x = -tval;
        y = tan((angle - 180) * pi / 180) * tval;
    }
    else if (angle <= 270) {
        y = -tval;
        x = -tan((270 - angle) * pi / 180) * tval;
    }
    else if (angle <= 315) {
        y = -tval;
        x = tan((angle - 270) * pi / 180) * tval;
    }
    else if (angle < 360) {
        x = tval;
        y = -tan((360 - angle) * pi / 180) * tval;
    }
    
    std::cout << y << std::endl;
    //test run coordinates
    glm::vec4 A(0.00f, 0.00f, 0.0f, 1.0f);
    glm::vec4 B(0.25f, 0.00f, 0.0f, 1.0f);
    glm::vec4 C(0.25f, 0.25f, 0.0f, 1.0f);
    glm::vec4 D(0.00f, 0.25f, 0.0f, 1.0f);

        
    while (!glfwWindowShouldClose(window)) {
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        closeOnInput(window);

        float timeValue = glfwGetTime();


        if ((count % 3) >= 0) {
            redValue = 1.0f;
            greenValue = 1.0f;
            blueValue = 0.0f;
        }
        if ((count % 3) >= 1) {
            redValue = 1.0f;
            greenValue = 0.0f;
            blueValue = 1.0f;
        }
        if ((count % 3) >= 2){
            redValue = 0.0f;
            greenValue = 1.0f;
            blueValue = 1.0f;
        }
        


        glUseProgram(shaderProgram);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        

        glUniform4f(colorLocation, redValue, greenValue, blueValue, 0.0f);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //get next translation 
        test = glm::translate(test, glm::vec3(x, y, 0.0f) * deltaTime);

        glm::mat4 MVP = projection * view * test;
        glm::vec4 screenPosA = MVP * A;
        screenPosA = (screenPosA / screenPosA.w) * 1000.0f;
        glm::vec4 screenPosB = MVP * B;
        screenPosB = screenPosB / screenPosB.w * 1000.0f;
        glm::vec4 screenPosC = MVP * C;
        screenPosC = screenPosC / screenPosC.w * 1000.0f;
        glm::vec4 screenPosD = MVP * D;
        screenPosD = screenPosD / screenPosD.w * 1000.0f;
        
        
        if (screenPosC.x > 1000 ) {
            x = -1 * x; 
            y = y;
            count++;
            //std::cout << count - 1 << " Right boundary " << std::endl;
        }
        else if (screenPosD.y > 1000) {
            x = x;
            y = -1 * y;
            count++;
            //std::cout <<count-1 <<" Top Boundary" << std::endl;
        }
        else if (screenPosA.x < -1000) {
            x = -1 * x;
            y = y;
            count++;
            //std::cout << count - 1 << " Left Boundary  " << std::endl;
        }
        else if (screenPosB.y < -1000) {
            x = x;
            y = -1 * y;
            count++;
            //std::cout << count - 1 << " Bottom Boundary  " << std::endl;
        }

        model = glm::translate(model, glm::vec3(x, y, 0.0f) * deltaTime);
        test = model;



        glBindVertexArray(VAO);
        glBindTexture(GL_TEXTURE_2D, texture);
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


/*
//3032720
//KRAPAH BLESS ELIKEM
#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include <cmath>


#include <iostream>

const int SRC_WIDTH = 800;
const int SRC_HEIGHT = 600;

//functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void closeOnInput(GLFWwindow* window);

const GLchar* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 Position;\n"
"layout (location = 1) in vec2 oTexCoord;\n"
"out vec2 sTexCoord;\n"
"uniform mat4 transform;"
"uniform mat4 model;"
"uniform mat4 view;"
"uniform mat4 projection;"
"void main()\n"
"{\n"
"   gl_Position = projection * view * model * vec4(Position, 1.0f);\n"
"	sTexCoord = vec2(oTexCoord.x, oTexCoord.y);\n"
"}\0";

const GLchar* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 sTexCoord;\n"
"uniform vec4 Color1;\n"
"uniform sampler2D ourTexture;\n"
"void main()\n"
"{\n"
"   FragColor = texture(ourTexture, sTexCoord) * Color1;\n"
"}\0";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "DVD Animation", NULL, NULL);
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

         0.00f, 0.00f, 0.0f,0.0f, 0.0f,
         0.25f, 0.25f, 0.0f,1.0f, 1.0f,
         0.25f, 0.00f, 0.0f,1.0f, 0.0f,

         0.00f, 0.00f, 0.0f,0.0f, 0.0f,
         0.25f, 0.25f, 0.0f,1.0f, 1.0f,
         0.00f, 0.25f, 0.0f,0.0f, 1.0f,
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // load and create a texture
    // -------------------------
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("C:/Users/ACE/Documents/COE 4/SEM 1/COMPUTER GRAPHICS/LABS/images/dvd-video-logo-png-transparent1.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //glUseProgram(shaderProgram);
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");

    GLuint colorLocation = glGetUniformLocation(shaderProgram, "Color1");

    //MVPPPPPPPPPPPP
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 test = glm::mat4(1.0f);

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f,
        100.0f);

    glm::mat4 view = glm::mat4(1.0f);

    // note that we’re translating the scene in the reverse direction
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));


        //auto t_start = std::chrono::high_resolution_clock::now();


        // Calculate transformation
        //auto t_now = std::chrono::high_resolution_clock::now();
        //float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();



    glUseProgram(shaderProgram);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    GLfloat currentFrame, deltaTime;
    float  lastFrame = 0.0f;

    float redValue;
    float greenValue;
    float blueValue;
    int count = 0;
    float pi = 3.141593, angle= 30;
    std::cout << "Enter angle to start projection" << std::endl;
    //std::cin >> angle;
    float x = 0.25f * 2;//0.01443f;
    float y = tan(angle * pi/180) * x;

    //test run coordinates
    glm::vec4 A(0.00f, 0.00f, 0.0f, 1.0f);
    glm::vec4 B(0.25f, 0.00f, 0.0f, 1.0f);
    glm::vec4 C(0.25f, 0.25f, 0.0f, 1.0f);
    glm::vec4 D(0.00f, 0.25f, 0.0f, 1.0f);


    while (!glfwWindowShouldClose(window)) {
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        closeOnInput(window);

        float timeValue = glfwGetTime();


        if ((count % 3) >= 0) {
            redValue = 1.0f;
            greenValue = 1.0f;
            blueValue = 0.0f;
        }
        if ((count % 3) >= 1) {
            redValue = 1.0f;
            greenValue = 0.0f;
            blueValue = 1.0f;
        }
        if ((count % 3) >= 2){
            redValue = 0.0f;
            greenValue = 1.0f;
            blueValue = 1.0f;
        }



        glUseProgram(shaderProgram);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


        glUniform4f(colorLocation, redValue, greenValue, blueValue, 0.0f);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //get next translation
        test = glm::translate(test, glm::vec3(x, y, 0.0f) * deltaTime);

        glm::mat4 MVP = projection * view * test;
        glm::vec4 screenPosA = MVP * A;
        screenPosA = (screenPosA / screenPosA.w) * 1000.0f;
        glm::vec4 screenPosB = MVP * B;
        screenPosB = screenPosB / screenPosB.w * 1000.0f;
        glm::vec4 screenPosC = MVP * C;
        screenPosC = screenPosC / screenPosC.w * 1000.0f;
        glm::vec4 screenPosD = MVP * D;
        screenPosD = screenPosD / screenPosD.w * 1000.0f;


        if (screenPosC.x > 1000 ) {
            x = -1 * x;
            y = y;
            count++;
            std::cout << count - 1 << " Left boundary " << std::endl;
        }
        else if (screenPosD.y > 1000) {
            x = x;
            y = -1 * y;
            count++;
            std::cout <<count-1 <<" Top Boundary" << std::endl;
        }
        else if (screenPosA.x < -1000) {
            x = -1 * x;
            y = y;
            count++;
            std::cout << count - 1 << " Right Boundary  " << std::endl;
        }
        else if (screenPosB.y < -1000) {
            x = x;
            y = -1 * y;
            count++;
            std::cout << count - 1 << " Bottom Boundary  " << std::endl;
        }

        model = glm::translate(model, glm::vec3(x, y, 0.0f) * deltaTime);
        test = model;



        glBindVertexArray(VAO);
        glBindTexture(GL_TEXTURE_2D, texture);
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

*/