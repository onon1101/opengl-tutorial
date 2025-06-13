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
#include <cstdlib>
#include <ctime>

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

	std::srand(std::time(nullptr));
	const int numParticles = 1000;
	float particleVertices[numParticles * 6];
	for (int i = 0; i < numParticles; ++i)
	{
		float x = static_cast<float>(std::rand()) / RAND_MAX * 2.0f - 1.0f;  // [-1, 1]
		float y = static_cast<float>(std::rand()) / RAND_MAX * 2.0f;         // [0, 2]
		float r = 1.0f;
		float g = 1.0f;
		float b = 1.0f;
		particleVertices[i * 6 + 0] = x;
		particleVertices[i * 6 + 1] = y;
		particleVertices[i * 6 + 2] = 0.0f;
		particleVertices[i * 6 + 3] = r;
		particleVertices[i * 6 + 4] = g;
		particleVertices[i * 6 + 5] = b;
	}

	// 每個 frame 動態更新粒子 Y 值
	float speed = 0.002f;

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
	// float vertices[] = {// position                         // color
	// 					0.0f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f,
	// 					0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
	// 					-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f};

	unsigned int VBO; // or GLuint. declare the value of the buffer id.
	unsigned int VAO; // declare vertex attribute object.
	// unsigned int EBO;
	glGenVertexArrays(1, &VAO); // just like vbo
	glGenBuffers(1, &VBO);		// generate vbo buffer id via opengl.
	// glGenBuffers(1, &EBO);

	glBindVertexArray(VAO); // binding

	glBindBuffer(GL_ARRAY_BUFFER, VBO); // binding VBO to GL_ARRAY_BUFFER.
	glBufferData(GL_ARRAY_BUFFER, sizeof(particleVertices), particleVertices, GL_STATIC_DRAW);
	/*
		linking vertex attr.
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
						  (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
						  (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	double lastTime = glfwGetTime();
	int nbFrames = 0;

	/*
		開始 loop to rendering
	*/
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0)
		{
			std::string title = "LearnOpengl - FPS: " + std::to_string(nbFrames);
			glfwSetWindowTitle(window, title.c_str());
			nbFrames = 0;
			lastTime = currentTime;
		}

		// 動態更新粒子 Y 值
		for (int i = 0; i < numParticles; ++i)
		{
			particleVertices[i * 6 + 1] -= speed;
			if (particleVertices[i * 6 + 1] < -1.0f)
			{
				particleVertices[i * 6 + 1] = 1.0f;
				particleVertices[i * 6 + 0] = static_cast<float>(std::rand()) / RAND_MAX * 2.0f - 1.0f;
			}
		}

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(particleVertices), particleVertices);

		// rendering
		glFinish(); // 強制 CPU 等待 GPU 完成，模擬頻繁同步造成的 bounding 問題
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // background
		glClear(GL_COLOR_BUFFER_BIT);

		// triangle
		shaderProgram.use();
		glPointSize(5.0f);

		// uniform
		glBindVertexArray(VAO);
		glDrawArrays(GL_POINTS, 0, numParticles);

		// checking
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO); 
	glDeleteBuffers(1, &VBO);
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