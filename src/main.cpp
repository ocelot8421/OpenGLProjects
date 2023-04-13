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

#include "io/joystick.h"
#include "io/keyboard.h"
#include "io/mouse.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);				
void processInput(GLFWwindow *window);

float mixVal = 0.5f;

glm::mat4 transform = glm::mat4(1.0f);

Joystick mainJ(0);

unsigned int SCR_WIDTH = 800, SCR_HEIGHT = 600;
float x, y, z;

int main() {
	std::cout << "... s t a r t ..." << std::endl;

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "First steps in Neuroimaging", NULL, NULL);	
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
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	// callback											
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //TODO (2) How can this work without call by parameters?

	glfwSetKeyCallback(window, Keyboard::keyCallback);

	glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);
	glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
	glfwSetScrollCallback(window, Mouse::mouseWheelCallback);
	
	// depth test, alias z-buffer
	glEnable(GL_DEPTH_TEST);

	// SHADERS=============================
	Shader shader("../assets/vertex_core.glsl", "../assets/fragment_core.glsl");
	
	//vertex array
	float vertices[] = {
		//position			  //texturecoordinates

		// back
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		//front
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		//left
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		//right
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		//bottom
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		//up
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

	// VAO, VBO
	unsigned int VAO, VBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	//bind VAO
	glBindVertexArray(VAO);
	//bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// SET VERTEX ATTRIBUTES
	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//texture coordinate
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

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
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("../assets/gekko_meretezett.jpg", &width, &height, &nChannels, 0);
	if(data){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cout << "Failed to load texture1." << std::endl;
	}
	stbi_image_free(data);

	//texture2
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	data = stbi_load("../assets/smile.png", &width, &height, &nChannels, 0);
	if(data){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cout << "Failed to load texture2." << std::endl;
	}
	stbi_image_free(data);

	shader.activate();
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);

	mainJ.update();
	if (mainJ.isPresent()) {
		std::cout << mainJ.getName() << "is present." << std::endl;
	} else {
		std::cout << " Joystick is not present." << std::endl;
	}
	
	x = 0.0f;
	y = 0.0f,
	z = 0.0f;

    while (!glfwWindowShouldClose(window)) {	
		processInput(window);												
		
		//render
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//texture1
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		//texture
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// draw shapes
		glBindVertexArray(VAO);

		// create transformation for screen
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view =glm::mat4(1.0f);
		glm::mat4 projection =glm::mat4(1.0f);

		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(0.5f)); //why 0.5 instead of 1.0? -- https://learnopengl.com/Getting-started/Transformations#:~:text=%2C%200.0%2C-,1.0,-))%3B%0Atrans%20%3D%20glm
		view = glm::translate(view, glm::vec3(x, y, z));
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		shader.activate();

		shader.setMat4("model", model);	
		shader.setMat4("view", view);	
		shader.setMat4("projection", projection);	
		shader.setFloat("mixVal", mixVal);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		// send new frame to window
        glfwSwapBuffers(window);											
        glfwPollEvents();													
    }

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	std::cout << "... e n d ..." << std::endl;

    glfwTerminate();												
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){	
    glViewport(0, 0, width, height);
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
}

void processInput(GLFWwindow *window) {										
    if(Keyboard::key(GLFW_KEY_ESCAPE)){
        glfwSetWindowShouldClose(window, true);
	}
	// change mixVal
	if (Keyboard::key(GLFW_KEY_UP))
	{
		mixVal += .05f;
		if (mixVal > 1){
			mixVal = 1.0f;
		}
	}	
	if (Keyboard::key(GLFW_KEY_DOWN))
	{
		mixVal -= .05f;
		if (mixVal < 0){
			mixVal = 0.0f;
		}
	}

	if (Keyboard::key(GLFW_KEY_W)) {
		transform = glm::translate(transform, glm::vec3(0.0f, 0.1f, 0.0f));
	}
	if (Keyboard::key(GLFW_KEY_S)) {
		transform = glm::translate(transform, glm::vec3(0.0f, -0.1f, 0.0f));
	}
	if (Keyboard::key(GLFW_KEY_A)) {
		transform = glm::translate(transform, glm::vec3(-0.1f, 0.1f, 0.0f));
	}
	if (Keyboard::key(GLFW_KEY_D)) {
		transform = glm::translate(transform, glm::vec3(0.1f, 0.0f, 0.0f));
	}

	//Joystick -- next time //TODO (0) 

	mainJ.update();	
}


