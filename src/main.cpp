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
	std::cout << "... s t a r t ..." << std::endl;

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

	//compile vertex shader													//learnopengl.com --  https://learnopengl.com/Getting-started/Hello-Triangle#:~:text=OpenGL%27s%20visible%20region.-,Compiling%20a%20shader,-We%20take%20the
	 unsigned int vertexShader;
	 vertexShader = glCreateShader(GL_VERTEX_SHADER);
	 std::string vertShaderSrc = loadShaderSrc("../assets/vertex_core.glsl");						// ../ because of bin file
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
	 const GLchar* fragShader = fragShaderSrc.c_str();
	 glShaderSource(fragmentShader, 1, &fragShader, NULL);
	 glCompileShader(fragmentShader);
	 //catch errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Error width fragment shader comp.: " << std::endl << infoLog << std::endl;
	}

	//link shader program													learnopengl.com -- https://learnopengl.com/Getting-started/Hello-Triangle#:~:text=here%20as%20well!-,Shader%20program,-A%20shader%20program
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
		0.5f, 0.5f, 0.0f,    	// '. °.	right	top
		-0.5f, 0.5f, 0.0f,		// °. '.	left	top
		-0.5f, -0.5f, 0.0f,		// 'o '.	left	bottom
		0.5f, -0.5f, 0.0f		// '. 'o	right	bottom
	};
	unsigned int indices[] = {
		0,1,2,					//first triangle
		2,3,0					//second triangle
	};


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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//set up EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    while (!glfwWindowShouldClose(window)) {								//RENDER LOOP (double buffer)
		processInput(window);												// takes the window as input together with a key
		//RENDERING COMMANDS
		glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		// draw shapes
		glBindVertexArray(VAO);
		glUseProgram(shaderProgram);
		//glDrawArrays(GL_TRIANGLES, 0, 3);									//triangle (3 points)
		//glDrawArrays(GL_TRIANGLES, 0, 6);									//rectangle (2 triangle, 6 points)
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);				//rectangle element

        glfwSwapBuffers(window);											//Swaps buffer that contains color values for each pixel in GLFW's window
        glfwPollEvents();													//checks keyboard input or mouse movement events, updates the window state, and calls the corresponding functions
    }

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &EBO);

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
	file.open(filename);													// learnopengl.com -- https://learnopengl.com/Getting-started/Shaders#:~:text=set%20its%20value.-,Reading%20from%20file,-We%27re%20using%20C
	if(file.is_open()) {
		buf << file.rdbuf();
		ret = buf.str();
	} else {
		std::cout << "Could not open " << filename << std::endl;
	}
	file.close(); 
	return ret;
}


