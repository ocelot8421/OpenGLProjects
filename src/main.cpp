#include <glad/glad.h> //this should be above GLFW header
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> //OpenGL Mathematics
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

//Adjusts the viewport to the window if the user resizes it
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//Checks inputs
void processInput(GLFWwindow *window);

int main() {
	//glm test
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);						//(1;0;0;1) vector
	glm::mat4 trans = glm::mat4(1.0f); 							//identity matrix ((1,0,0,0);(0,1,0,0);(0,0,1,0);(0,0,0,1))
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f)); //translation (identityMatrix, translationBy(1,1,0))
	vec = trans * vec;											//
	std::cout << vec.x << vec.y << vec.z << std::endl;


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

		//RENDERING COMMANDS
		// clear the screen's color buffer
        //glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
		//Swaps buffer that contains color values for each pixel in GLFW's window
        glfwSwapBuffers(window);
		//checks keyboard input or mouse movement events, updates the window state, and calls the corresponding functions 
        glfwPollEvents();
 
 
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