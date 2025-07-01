// clang-format off
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"
#include <iostream>
// clang-format on
constexpr unsigned int SRC_WIDTH = 800;
constexpr unsigned int SRC_HEIGHT = 600;

int
main()
{
	// glfw initialize and configure.
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creatation
	GLFWwindow *window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		spdlog::error("Failed to create GLFW window");
		glfwTerminate();
		return -1;
	} 
	glfwMakeContextCurrent(window);

	// glad to manage the pointer of opengl
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		spdlog::error("Failed to initialize GLAD");
		return -1;
	}
	return 0;
}