#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

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
	//  initialize, then configure Graphics Library FrameWork
    glfwInit();
	if (!glfwInit())
	{
		std::cout << "GLFW initialization failed." << std::endl;
	}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);							
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//  Creating a window and context
    GLFWwindow* window = glfwCreateWindow(640, 480, "First steps in Neuroimaging", NULL, NULL);	
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    } 
    glfwMakeContextCurrent(window);

	//glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {				
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

	//viewport
	glViewport(0, 0, 640, 480);												
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //TODO (2) How can this work without call by parameters?

	Shader shader("../assets/vertex_core.glsl", "../assets/fragment_core.glsl");
	Shader shader2("../assets/vertex_core.glsl", "../assets/fragment_core2.glsl");
	
	//vertex array
	float vertices[]  = {
		// position				//colors			//texture coordinates
		 0.5f,  0.5f, 0.0f,    	1.0f, 1.0f, 0.5f,	0.0f, 0.0f,		// '. °.	right	top		
		-0.5f,  0.5f, 0.0f,		0.5f, 1.0f, 0.7f,	0.0f, 1.0f,		// °. '.	left	top
		-0.5f, -0.5f, 0.0f,		0.6f, 1.0f, 0.2f,	1.0f, 0.0f,		// 'o '.	left	bottom
		 0.5f, -0.5f, 0.0f,		1.0f, 0.2f,	1.0f,	1.0f, 1.0f		// '. 'o	right	bottom
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

	// SET VERTEX ATTRIBUTES
	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//texture coordinate
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// TEXTURES

	// generating texture
	unsigned int texture1;

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);		// https://learnopengl.com/Getting-started/Textures#:~:text=per%20coordinate%20axis-,(s%2C%20t%20(,-and%20r%20if
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// load image
	int width, height, nChannels;
	unsigned char* data = stbi_load("../assets/pic1_kek.jpg", &width, &height, &nChannels, 0);

	if(data){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cout << "Failed to load texture." << std::endl;
	}
	stbi_image_free(data);

	shader.activate();
	shader.setInt("texture1", 0);


	//set up EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.activate();
	shader.setMat4("transform", trans);

	glm::mat4 trans2 = glm::mat4(1.0f);
	trans2 = glm::scale(trans2, glm::vec3(0.6f));
	trans2 = glm::rotate(trans2, glm::radians(15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader2.activate();
	shader2.setMat4("transform", trans);

    while (!glfwWindowShouldClose(window)) {	
		processInput(window);												
		
		//rendering
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		trans = glm::rotate(trans, glm::radians((float)glfwGetTime() / 25.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		shader.activate();
		shader.setMat4("transform", trans);
		//shader2.activate();
		//shader2.setMat4("transform", trans);

		// draw shapes
		glBindVertexArray(VAO);
		shader.activate();
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		trans2 = glm::rotate(trans2, glm::radians((float)glfwGetTime() / -50.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		shader2.activate();
		shader2.setMat4("transform", trans2);

		shader2.activate();
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * sizeof(GLuint)));


		glBindVertexArray(0);

        glfwSwapBuffers(window);											
        glfwPollEvents();													
    }

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &EBO);

    glfwTerminate();	//destroys any remaining windows and releases any other resources allocated by GLFW													
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){	
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {										
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
