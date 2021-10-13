//
//  main.cpp
//  Colors
//
//  Created by 陈学明 on 2021/10/12.
//
#include <iostream>
#include <dirent.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <CoreFoundation/CoreFoundation.h>

#include "utils/LamplightShader.hpp"
#include "utils/ShaderStream.hpp"
#include "utils/Shader.hpp"
#include "Camera.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

void procesInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}


// 变换
int main(int argc, const char * argv[]) {
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "learn shaders", NULL, NULL);
    if (window == NULL) {
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        return -1;
    }
    
    // 着色器程序
    ShaderStream *shaderCode = new ShaderStream();
    Shader lightingShader = Shader(shaderCode);
    
    LamplightShader *lampCode = new LamplightShader();
    Shader lightCubeShader = Shader(lampCode);
    
    // set up vertex data (and buffer(s)) and configure vertex attributes
      // ------------------------------------------------------------------
      float vertices[] = {
          -0.5f, -0.5f, -0.5f,
           0.5f, -0.5f, -0.5f,
           0.5f,  0.5f, -0.5f,
           0.5f,  0.5f, -0.5f,
          -0.5f,  0.5f, -0.5f,
          -0.5f, -0.5f, -0.5f,

          -0.5f, -0.5f,  0.5f,
           0.5f, -0.5f,  0.5f,
           0.5f,  0.5f,  0.5f,
           0.5f,  0.5f,  0.5f,
          -0.5f,  0.5f,  0.5f,
          -0.5f, -0.5f,  0.5f,

          -0.5f,  0.5f,  0.5f,
          -0.5f,  0.5f, -0.5f,
          -0.5f, -0.5f, -0.5f,
          -0.5f, -0.5f, -0.5f,
          -0.5f, -0.5f,  0.5f,
          -0.5f,  0.5f,  0.5f,

           0.5f,  0.5f,  0.5f,
           0.5f,  0.5f, -0.5f,
           0.5f, -0.5f, -0.5f,
           0.5f, -0.5f, -0.5f,
           0.5f, -0.5f,  0.5f,
           0.5f,  0.5f,  0.5f,

          -0.5f, -0.5f, -0.5f,
           0.5f, -0.5f, -0.5f,
           0.5f, -0.5f,  0.5f,
           0.5f, -0.5f,  0.5f,
          -0.5f, -0.5f,  0.5f,
          -0.5f, -0.5f, -0.5f,

          -0.5f,  0.5f, -0.5f,
           0.5f,  0.5f, -0.5f,
           0.5f,  0.5f,  0.5f,
           0.5f,  0.5f,  0.5f,
          -0.5f,  0.5f,  0.5f,
          -0.5f,  0.5f, -0.5f,
      };
      // first, configure the cube's VAO (and VBO)
      unsigned int VBO, cubeVAO;
      glGenVertexArrays(1, &cubeVAO);
      glGenBuffers(1, &VBO);

      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

      glBindVertexArray(cubeVAO);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);

      unsigned int lightCubeVAO;
      glGenVertexArrays(1, &lightCubeVAO);
      glBindVertexArray(lightCubeVAO);

      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);


      // render loop
      // -----------
      while (!glfwWindowShouldClose(window))
      {
          // per-frame time logic
          // --------------------
          float currentFrame = glfwGetTime();
          deltaTime = currentFrame - lastFrame;
          lastFrame = currentFrame;

          // input
          // -----
          procesInput(window);

          // render
          // ------
          glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
          
          // be sure to activate shader when setting uniforms/drawing objects
          lightingShader.use();
          lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
          lightingShader.setVec3("lightColor",  1.0f, 1.0f, 1.0f);

          // view/projection transformations
          glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
          glm::mat4 view = camera.GetViewMatrix();
          lightingShader.setMat4("projection", projection);
          lightingShader.setMat4("view", view);

          // world transformation
          glm::mat4 model = glm::mat4(1.0f);
          lightingShader.setMat4("model", model);

          // render the cube
          glBindVertexArray(cubeVAO);
          glDrawArrays(GL_TRIANGLES, 0, 36);


          // also draw the lamp object
          lightCubeShader.use();
          lightCubeShader.setMat4("projection", projection);
          lightCubeShader.setMat4("view", view);
          model = glm::mat4(1.0f);
          model = glm::translate(model, lightPos);
          model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
          lightCubeShader.setMat4("model", model);
          
          glBindVertexArray(lightCubeVAO);
          glDrawArrays(GL_TRIANGLES, 0, 36);


          // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
          // -------------------------------------------------------------------------------
          glfwSwapBuffers(window);
          glfwPollEvents();
      }

      glDeleteVertexArrays(1, &cubeVAO);
      glDeleteVertexArrays(1, &lightCubeVAO);
      glDeleteBuffers(1, &VBO);

      glfwTerminate();
      return 0;
    return 0;
}
