/*
GLFW 會用到 opengl 的東西，因此需要先導入 glad。
*/
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int witdh, int height);
void processInput(GLFWwindow* window);

int main(int, char**) {

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

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpengl", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    /*
        使用 glad 管理 opengl pointer。
    */

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    /*
        設定 viewport。
    */
    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    /* 
    開始 loop to rendering
    */
    while(!glfwWindowShouldClose(window)) {
        // input
        processInput(window);

        //rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // checking        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int witdh, int height) {
    glViewport(0, 0, witdh, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}