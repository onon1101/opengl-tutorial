/*
	GLFW 會用到 opengl 的東西，因此需要先導入 glad。
*/
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Shader.hpp"

void
framebuffer_size_callback(GLFWwindow *window, int witdh, int height);
void
processInput(GLFWwindow *window);

int
main(int, char **)
{

	/*
		設定 glfw
	   GLFW_CONTEXT_VERSION_MAJOR: 設定版本 3
	   GLFW_OPENGL_PROFILE: without backwards-compatible features
	*/
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	/*
		設定與啟動 window
	*/

	GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpengl", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	/*
		使用 glad 管理 opengl pointer。
	*/

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	/*
		設定 viewport。
	*/
	int fbWidth, fbHeight;
	glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
	glViewport(0, 0, fbWidth, fbHeight);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	/*
		設定 shader.
	*/
	Shader shaderProgram(SOURCE_DIR "shader.vert", SOURCE_DIR "shader.frag");

	/*
		設定 vertices buffer and vertex attribute。
	*/
	float vertices[] = {// position                         // color
						0.0f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f,
						0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
						-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f};

	unsigned int VBO; // or GLuint. declare the value of the buffer id.
	unsigned int VAO; // declare vertex attribute object.
	// unsigned int EBO;
	glGenVertexArrays(1, &VAO); // just like vbo
	glGenBuffers(1, &VBO);		// generate vbo buffer id via opengl.
	// glGenBuffers(1, &EBO);

	glBindVertexArray(VAO); // binding

	glBindBuffer(GL_ARRAY_BUFFER, VBO); // binding VBO to GL_ARRAY_BUFFER.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	/*
		linking vertex attr.
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
						  (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
						  (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	/*
		開始 loop to rendering
	*/
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // background
		glClear(GL_COLOR_BUFFER_BIT);

		// triangle
		shaderProgram.use();

		// uniform
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// checking
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

void
framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void
processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}