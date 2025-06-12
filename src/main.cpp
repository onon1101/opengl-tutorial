/*
GLFW 會用到 opengl 的東西，因此需要先導入 glad。
*/
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);
    // glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    /* 
        設定 vertices and vertex attribute。
    */
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    unsigned int VBO; // or GLuint. declare the value of the buffer id.
    unsigned int VAO; // declare vertex attribute object.
    glGenBuffers(1, &VBO); // generate vbo buffer id via opengl.
    glGenVertexArrays(1, &VAO); // just like vbo
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // binding VBO to GL_ARRAY_BUFFER.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO); // binding 

    /*
        read vertces shader.
    */
    std::ifstream file(SOURCE_DIR "shader.vert");
    if (!file.is_open()) {
        std::cerr << "Shader 檔案無法開啟" << std::endl;
        return -1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string shader_str = buffer.str();
    const char* vertexShaderSource = shader_str.c_str();

    /*
        create and compile shader.
    */
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    /*
        read, create and compile fragment shader.
    */
    std::ifstream file02(SOURCE_DIR "shader.frag");
    std::stringstream buffer02;
    buffer02 << file02.rdbuf();
    std::string shader_str02 = buffer02.str();
    const char* fragmentShaderSource = shader_str02.c_str();
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    /*
        create shader program.
    */
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    /* 
        delete vertex and fragment shader
    */
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    /*
        linking vertex attr.
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 *sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    /* 
        開始 loop to rendering
    */
    while(!glfwWindowShouldClose(window)) {
        // input
        processInput(window);

        //rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // checking        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}