//
//  main.cpp
//  Shaders
//
//  Created by 陈学明 on 2021/7/1.
//

#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include "utils/ShaderStream.hpp"
#include "utils/Shader.hpp"
#include <assimp/ai_assert.h>
#define _SHADER_SRC(...) #__VA_ARGS__
#define SHADER_SRC(...) _SHADER_SRC(__VA_ARGS__)

const int Window_width = 600;
const int Window_height = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void procesInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main(int argc, const char * argv[]) {
    // 初始化窗口
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow *window = glfwCreateWindow(Window_width, Window_height, "learn shaders", NULL, NULL);
    if (window == NULL) {
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        return -1;
    }
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    while (!glfwWindowShouldClose(window)) {
        procesInput(window);
        glClearColor(0.2, 0.2, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    std::cout << "Hello, World!\n";
    glfwTerminate();
    return 0;
}

