#include <glad/glad.h> //this should be above GLFW header
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);				
void processInput(GLFWwindow *window);													

int main() {
	std::cout << "... s t a r t ..." << std::endl;

	int success;															
	char infoLog[512];

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);							
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);			
    GLFWwindow* window = glfwCreateWindow(900, 600, "First steps in Neuroimaging", NULL, NULL);	
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    } 
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {				
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
	glViewport(0, 0, 900, 600);												
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);		

	Shader shader("../assets/vertex_core.glsl", "../assets/fragment_core.glsl");
	
	//vertex array
	float vertices[]  = {
		// position				//colors
		 0.5f,  0.5f, 0.0f,    	1.0f, 1.0f, 0.5f,	// '. °.	right	top		
		-0.5f,  0.5f, 0.0f,		0.5f, 1.0f, 0.7f,	// °. '.	left	top
		-0.5f, -0.5f, 0.0f,		0.6f, 1.0f, 0.2f,	// 'o '.	left	bottom
		 0.5f, -0.5f, 0.0f,		1.0f, 0.2f,	1.0f	// '. 'o	right	bottom
	};
	unsigned int indices[] = {
		0,1,2,					//first triangle
		2,3,0					//second triangle
	};

	//float vertices[]  = {
	//	// position
	//	-0.4f,  0.5f, 0.0f,		// °. '.	left	top
	//	-0.5f, -0.5f, 0.0f,		// 'o '.	left	bottom
	//	-0.3f, -0.5f, 0.0f,		// '. 'o	right	bottom

	//	  0.4f,  0.5f, 0.0f,    // '. °.	right	top
	//	  0.3f, -0.5f, 0.0f,	// 'o '.	left	bottom
	//	  0.5f, -0.5f, 0.0f		// '. 'o	right	bottom
	//};
	//unsigned int indices[] = {
	//	0,1,2,					//first triangle
	//	3,4,5					//second triangle
	//};

	// VAO, VBO
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	//bind VAO
	glBindVertexArray(VAO);
	//bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// set attribute pointer
	
	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//set up EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.activate();
	shader.setMat4("transform", trans);

    while (!glfwWindowShouldClose(window)) {	
		processInput(window);												
		
		//rendering
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		trans = glm::rotate(trans, glm::radians((float)glfwGetTime() / 25.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		shader.activate();
		shader.setMat4("transform", trans);

		// draw shapes
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);				
		glBindVertexArray(0);

        glfwSwapBuffers(window);											
        glfwPollEvents();													
    }

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &EBO);

    glfwTerminate();														
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){	
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {										
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
