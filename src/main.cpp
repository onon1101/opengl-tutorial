#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

// 載入 shader
std::string loadShaderSource(const char* path) {
    std::ifstream file(path);
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

// 編譯 shader
GLuint compileShader(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    // 檢查編譯錯誤
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation error:\n" << infoLog << std::endl;
    }

    return shader;
}

// 初始化 shader 程式
GLuint initShaderProgram() {
    std::string vertSrc = loadShaderSource("/Users/chenshiang/Desktop/dev/opengl-tutorial/src/shader.vert");
    std::string fragSrc = loadShaderSource("/Users/chenshiang/Desktop/dev/opengl-tutorial/src/shader.frag");
    GLuint vert = compileShader(GL_VERTEX_SHADER, vertSrc.c_str());
    GLuint frag = compileShader(GL_FRAGMENT_SHADER, fragSrc.c_str());

    GLuint program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    glDeleteShader(vert);
    glDeleteShader(frag);
    return program;
}

// 全螢幕矩形（兩個三角形）
float vertices[] = {
    // positions      // tex coords
    -1.0f,  1.0f,      0.0f, 1.0f,
    -1.0f, -1.0f,      0.0f, 0.0f,
     0.0f, -1.0f,      0.5f, 0.0f,

    -1.0f,  1.0f,      0.0f, 1.0f,
     0.0f, -1.0f,      0.5f, 0.0f,
     0.0f,  1.0f,      0.5f, 1.0f,

     0.0f,  1.0f,      0.5f, 1.0f,
     0.0f, -1.0f,      0.5f, 0.0f,
     1.0f, -1.0f,      1.0f, 0.0f,

     0.0f,  1.0f,      0.5f, 1.0f,
     1.0f, -1.0f,      1.0f, 0.0f,
     1.0f,  1.0f,      1.0f, 1.0f
};

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Gamma Test", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    GLuint shaderProgram = initShaderProgram();

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO); glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); // 位置
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))); // UV
    glEnableVertexAttribArray(1);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}