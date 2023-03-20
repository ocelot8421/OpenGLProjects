#include <glad/glad.h> //this should be above GLFW header
#include <GLFW/glfw3.h>

#include <iostream>

//Adjusts the viewport to the window if the user resizes it
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//Checks inputs
void processInput(GLFWwindow *window);

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

	//rendering window with its dimensions (lower left corner's coordinates, width, height)
	glViewport(0, 0, 900, 600);

	//Adjusts the viewport to the window if the user resizes it
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
 

	//RENDER LOOP (double buffer)
    while (!glfwWindowShouldClose(window)) {
		// takes the window as input together with a key
		processInput(window);
        
		//Swaps buffer that contains color values for each pixel in GLFW's window
        glfwSwapBuffers(window);
		//checks keyboard input or mouse movement events, updates the window state, and calls the corresponding functions 
        glfwPollEvents();
 
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
 
    }
 
	//clean,delete all of GLFW's resources
    glfwTerminate();
    return 0;
}

//Adjusts the viewport to the window if the user resizes it
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

// takes the window as input together with a key
void processInput(GLFWwindow *window)
{
	// checks whether the user has pressed the escape key
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}