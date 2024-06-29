#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void processInput(GLFWwindow* window);

int main1(void) 
{
	std::cout << "Hello";

	//create window

	//initialize GLFW
	if (!glfwInit()) {
		std::cout << "Failed";
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout<<"Failed To Load GLAD";
		return -1;
	}

	//create window object
	GLFWwindow* window = glfwCreateWindow(800, 600, "Trial", NULL, NULL);
	if (window == NULL) {
		std::cout<<"FAILED TO CREATE WINDOW OBJ";
	}

	glViewport(0, 0, 800, 600);

	while (!glfwWindowShouldClose) {
		processInput(window);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ENTER)==GLFW_PRESS);
	glfwSetWindowShouldClose(window, 1);
}