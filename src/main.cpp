#include <glad/glad.h> //this should be above GLFW header
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>
#include <algorithm>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
std::string loadShaderSrc(const char *filename);

bool comp(int f1, int f2);

int main()
{
	std::cout << "main starting..." << std::endl;

	float widthWindow = 900;
	float heightWindow = 600;
	float margin = 10;
	float widthView = widthWindow - 2 * margin;
	float heightView = heightWindow - 2 * margin;

	char markers[] = {'a',
					  'b',
					  'c'};
	float sides[3];

	for (int i = 0; i < std::size(markers); i++)
	{
		std::cout << "side " << markers[i] << ":" << std::endl;
		float s;
		std::cin >> s;
		sides[i] = s;
	}

	float a = sides[0];
	float b = sides[1];
	float c = sides[2];
	float delta = 0;

	int success;
	char infoLog[512];

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *window = glfwCreateWindow(widthWindow, heightWindow, "edag", NULL, NULL); // create a window (on Windows OS), params: width, height, title, monitor specifications? //TODO (0) monitor specifications?
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glViewport(margin, margin, widthView, heightView);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	/*
		shaders
	*/

	// compile vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::string vertShaderSrc = loadShaderSrc("../assets/vertex_core.glsl");
	const GLchar *vertShader = vertShaderSrc.c_str();
	glShaderSource(vertexShader, 1, &vertShader, NULL);
	glCompileShader(vertexShader);
	// catch errors
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Error width vertex shader comp.: " << std::endl
				  << infoLog << std::endl;
	}

	// compile fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragShaderSrc = loadShaderSrc("../assets/fragment_core.glsl");
	const GLchar *fragShader = fragShaderSrc.c_str();
	glShaderSource(fragmentShader, 1, &fragShader, NULL);
	glCompileShader(fragmentShader);
	// catch errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Error width fragment shader comp.: " << std::endl
				  << infoLog << std::endl;
	}

	// link
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// catch errors
	glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "Linking error: " << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	/* shaders*/

	float a_b = (b * b - a * a - c * c) / (-2 * a); // b oldal vetülete a-ra (nominális)
	float a_c = -a + a_b;							// c oldal vetülete a-ra
	float mA = std::sqrt(c * c - a_b * a_b);

	float w = std::max({a, a_b, a_c}, comp);
	std::cout << a << std::endl;
	std::cout << a_b << std::endl;
	std::cout << a_c << std::endl;
	std::cout << w << std::endl;
	float difOx = w / 2 - a_b;

	float Bx = (a_b + difOx) / widthView;
	float Ax = (a_c + difOx) / widthView;
	float Cx = (0 + difOx) / widthView;

	float Cy = mA / 2 / heightView;
	float By = -Cy;
	float Ay = By;

	// vertex array
	float vertices[] = {
		Bx, By, 0.0f,
		Ax, Ay, 0.0f,
		Cx, Cy, 0.0f};

	std::cout << "A: " << Ax << ", " << Ay << "\n"
			  << "B: " << Bx << ", " << By << "\n"
			  << "C: " << Cx << ", " << Cy << std::endl;
	// VAO, VBO
	unsigned int VAO;
	unsigned int VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind VAO
	glBindVertexArray(VAO);
	// bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// set attribute pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		// RENDERING COMMANDS
		glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw shapes
		glBindVertexArray(VAO);
		glUseProgram(shaderProgram);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	// checks whether the user has pressed the escape key
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

std::string loadShaderSrc(const char *filename)
{
	std::ifstream file;
	std::stringstream buf;
	std::string ret = "";
	file.open(filename);
	if (file.is_open())
	{
		buf << file.rdbuf();
		ret = buf.str();
	}
	else
	{
		std::cout << "Could not open... " << filename << std::endl;
	}
	file.close();
	return ret;
}

bool comp(int f1, int f2)
{
	return (f1 < f2);
}
