#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

int main() {
    glfwInit();

	// openGL version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//use openGL core-profile 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
 
	// create a window (on Windows OS)
	// params: width, height, title, monitor specifications? //TODO (0) monitor specifications
    GLFWwindow* window = glfwCreateWindow(900, 600, "First steps in Neuroimaging", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    } 
    glfwMakeContextCurrent(window);
 
	//Checks if Glad is loaded
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

	//glViewport(0, 0, 900, 600);
 
    while (!glfwWindowShouldClose(window)) {
        
        glfwPollEvents();
 
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
 
        glfwSwapBuffers(window);
    }
 
    glfwTerminate();
    return 0;
}