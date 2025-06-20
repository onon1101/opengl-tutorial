// clang-format off
#include <cstdlib>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const GLuint HEIGHT = 600;
const GLuint WIDTH = 800;

// Callback function for window resize events
void
framebuffer_size_callback(GLFWwindow *window, int width, int height);

void
checkCompileError(GLuint shader, const char *type);
int
main(int, char **)
{

	// settings and initialize glfw.
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// create window.
	GLFWwindow *window =
		glfwCreateWindow(WIDTH, HEIGHT, "LearningOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// use glad to manage opengl pointer.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// settings viewport.
	int fbWidth, fbHeight;
	glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
	glViewport(0, 0, fbWidth, fbHeight);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// read shader file.
	const char *vertexShaderPath = SOURCE_DIR "shader.vert";
	const char *fragmentShaderPath = SOURCE_DIR "shader.frag";

	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;

	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit |
								  std::ifstream::badbit);

	vertexShaderFile.open(vertexShaderPath);
	fragmentShaderFile.open(fragmentShaderPath);

	std::stringstream vertexShaderStream;
	std::stringstream fragmentShaderStream;

	vertexShaderStream << vertexShaderFile.rdbuf();
	fragmentShaderStream << fragmentShaderFile.rdbuf();

	vertexShaderFile.close();
	fragmentShaderFile.close();

	std::string vertexCode = vertexShaderStream.str();
	std::string fragmentCode = fragmentShaderStream.str();

	const char *vertexShaderCode = vertexCode.c_str();
	const char *fragmentShaderCode = fragmentCode.c_str();

	// create shader file in opengl.
	GLuint vertex;
	GLuint fragment;

	vertex = glCreateShader(GL_VERTEX_SHADER); // create vertex shader.
	glShaderSource(vertex, 1, &vertexShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileError(vertex, "VERTEX");

	fragment = glCreateShader(GL_FRAGMENT_SHADER); // create fragment shader.
	glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileError(fragment, "FRAGMENT");

	GLuint shaderProgramID; // create shader program.
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertex);
	glAttachShader(shaderProgramID, fragment);
	glLinkProgram(shaderProgramID);
	checkCompileError(shaderProgramID, "PROGRAM");

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	stbi_set_flip_vertically_on_load(true);

	int imageWidth, imageHeight, nrChannels;
	unsigned char *data = stbi_load(ASSETS_DIR "image.jpg", &imageWidth,
									&imageHeight, &nrChannels, 0);

	// texture
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0,
					 GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// VAO, VBO, EBO
	float vertices[] = {
		// positions          // colors           // texture coords
		0.2f,  0.2f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
		0.2f,  -0.2f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
		-0.2f, -0.2f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
		-0.2f, 0.2f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3	 // second triangle
	};

	// clang-format on
	unsigned int VBO; // or GLuint. declare the value of the buffer id.
	unsigned int VAO; // declare vertex attribute object.
	unsigned int EBO;
	glGenVertexArrays(1, &VAO); // just like vbo
	glGenBuffers(1, &VBO);		// generate vbo buffer id via opengl.
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO); // binding

	glBindBuffer(GL_ARRAY_BUFFER, VBO); // binding VBO to GL_ARRAY_BUFFER.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
				 GL_STATIC_DRAW);

	/*
		linking vertex attr.
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
						  (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
						  (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
						  (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glUniform1i(glGetUniformLocation(shaderProgramID, "ourTexture"), 0);

	float posX = 0.0f, posY = 0.0f;
	float speedX = ((float)rand() / RAND_MAX - 0.5f) * 0.02f;
	float speedY = ((float)rand() / RAND_MAX - 0.5f) * 0.02f;

	while (!glfwWindowShouldClose(window))
	{
		// polling key input
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}

		// rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgramID);

		posX += speedX;
		posY += speedY;

		if (posX >= 1.0f - 0.2f || posX <= -1.0f + 0.2f)
		{
			speedX *= -1;
			float jitter = ((float)rand() / RAND_MAX - 0.5f) * 0.02f; // ±0.01
			speedY += jitter;
		}
		if (posY > 1.0f - 0.2f || posY < -1.0f + 0.2f)
		{
			speedY *= -1;
			float jitter = ((float)rand() / RAND_MAX - 0.5f) * 0.02f;
			speedX += jitter;
		}

		glUniform2f(glad_glGetUniformLocation(shaderProgramID, "movePos"), posX,
					posY);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// checking
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// erase data.
	glDeleteProgram(shaderProgramID);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();
	return 0;
}

void
framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void
checkCompileError(GLuint shader, const char *type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout
				<< "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
				<< infoLog
				<< "\n -- --------------------------------------------------- "
				   "-- "
				<< std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout
				<< "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
				<< infoLog
				<< "\n -- --------------------------------------------------- "
				   "-- "
				<< std::endl;
		}
	}
}