#include <glad/glad.h> //this should be above GLFW header
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);				//Adjusts the viewport to the window if the user resizes it
void processInput(GLFWwindow *window);													//Checks inputs
std::string loadShaderSrc(const char* filename);

int main() {
	std::cout << "main starting..." << std::endl;

	int success;															//for exeption handling of vertex shader
	char infoLog[512];

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);										// openGL version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);						//use openGL core-profile
    GLFWwindow* window = glfwCreateWindow(900, 600, "First steps in Neuroimaging", NULL, NULL);		// create a window (on Windows OS), params: width, height, title, monitor specifications? //TODO (0) monitor specifications?
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    } 
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {				//Checks if Glad is loaded
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
	glViewport(0, 0, 900, 600);												//rendering window with its dimensions (lower left corner's coordinates, width, height)
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);		//Adjusts the viewport to the window if the user resizes it

	/*
		shaders																//TODO understand what it does exactly do.
	*/																		// VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV

	//compile vertex shader
	 unsigned int vertexShader;
	 vertexShader = glCreateShader(GL_VERTEX_SHADER);
	 std::string vertShaderSrc = loadShaderSrc("../assets/vertex_core.glsl");						// ../ because of bin file
	 std::cout << "vertShaderSrc: " << vertShaderSrc << std::endl; //TODO ---
	 const GLchar* vertShader = vertShaderSrc.c_str();
	 glShaderSource(vertexShader, 1, &vertShader, NULL);
	 glCompileShader(vertexShader);
	 //catch errors
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Error width vertex shader comp.: " << std::endl << infoLog << std::endl;
	}
	
	//compile fragment shader
	 unsigned int fragmentShader;
	 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	 std::string fragShaderSrc = loadShaderSrc("../assets/fragment_core.glsl");						// ../ because of bin file
	 std::cout << "fragShaderSrc: " << fragShaderSrc << std::endl; //TODO ---
	 const GLchar* fragShader = fragShaderSrc.c_str();
	 glShaderSource(fragmentShader, 1, &fragShader, NULL);
	 glCompileShader(fragmentShader);
	 //catch errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Error width fragment shader comp.: " << std::endl << infoLog << std::endl;
	}

	//link
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader); 
	glLinkProgram(shaderProgram);
	 //catch errors
	glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "Linking error: " << std::endl << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);											// VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
	/* shaders*/															//TODO understand what it does exactly do.

	//vertex array
	float vertices[]  = {
		-0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f
	};
	// VAO, VBO
	unsigned int VAO;
	unsigned int VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//bind VAO
	glBindVertexArray(VAO);
	//bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// set attribute pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


    while (!glfwWindowShouldClose(window)) {								//RENDER LOOP (double buffer)
		processInput(window);												// takes the window as input together with a key
		//RENDERING COMMANDS
		glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		// draw shapes
		glBindVertexArray(VAO);
		glUseProgram(shaderProgram);
		glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);											//Swaps buffer that contains color values for each pixel in GLFW's window
        glfwPollEvents();													//checks keyboard input or mouse movement events, updates the window state, and calls the corresponding functions
    }
    glfwTerminate();														//clean,delete all of GLFW's resources
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){	//Adjusts the viewport to the window if the user resizes it
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {										// takes the window as input together with a key
	// checks whether the user has pressed the escape key
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

std::string loadShaderSrc(const char* filename){
	std::ifstream file;
	std::stringstream buf;
	std::string ret = "";
	file.open(filename); //TODO (5) I/O
	if(file.is_open()) {
		buf << file.rdbuf();
		ret = buf.str();
	} else {
		std::cout << "Could not open " << filename << std::endl;
	}
	file.close(); 
	return ret;
}


