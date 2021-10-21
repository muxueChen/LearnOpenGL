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
#include "utils/UFImage.h"
#include <map>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 2.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// 变换
int main(int argc, const char * argv[]) {
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        return -1;
    }
    
    // 着色器程序
    ShaderStream *shaderCode = new ShaderStream();
    Shader shader = Shader(shaderCode);
    
    // ------------------------------------------------------------------
      float cubeVertices[] = {
          // positions          // texture Coords
          -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
           0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
           0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
           0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
          -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
          -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

          -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
           0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
           0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
           0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
          -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
          -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

          -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
          -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
          -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
          -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
          -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
          -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

           0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
           0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
           0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
           0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
           0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
           0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

          -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
           0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
           0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
           0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
          -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
          -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

          -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
           0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
           0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
           0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
          -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
          -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
      };
      float planeVertices[] = {
          // positions          // texture Coords
           5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
          -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
          -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

           5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
          -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
           5.0f, -0.5f, -5.0f,  2.0f, 2.0f
      };
      float transparentVertices[] = {
          // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
          0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
          0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
          1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

          0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
          1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
          1.0f,  0.5f,  0.0f,  1.0f,  0.0f
      };
      // cube VAO
      unsigned int cubeVAO, cubeVBO;
      glGenVertexArrays(1, &cubeVAO);
      glGenBuffers(1, &cubeVBO);
      glBindVertexArray(cubeVAO);
      glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
      // plane VAO
      unsigned int planeVAO, planeVBO;
      glGenVertexArrays(1, &planeVAO);
      glGenBuffers(1, &planeVBO);
      glBindVertexArray(planeVAO);
      glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
      // transparent VAO
      unsigned int transparentVAO, transparentVBO;
      glGenVertexArrays(1, &transparentVAO);
      glGenBuffers(1, &transparentVBO);
      glBindVertexArray(transparentVAO);
      glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
      glBindVertexArray(0);

      unsigned int cubeTexture = UFImage("/Users/chenxueming/Desktop/LearnOpenGL/DepthTest/DepthTest/container2.png").glTexture();
      unsigned int floorTexture = UFImage("/Users/chenxueming/Desktop/LearnOpenGL/DepthTest/DepthTest/container.jpeg").glTexture();
      unsigned int transparentTexture = UFImage("/Users/chenxueming/Desktop/LearnOpenGL/DepthTest/DepthTest/grass.png").glTexture();
    unsigned int windowTexture = UFImage("/Users/chenxueming/Desktop/LearnOpenGL/DepthTest/DepthTest/window.png").glTexture();

    // transparent window locations
       // --------------------------------
       vector<glm::vec3> windows
       {
           glm::vec3(-1.5f, 0.0f, -0.48f),
           glm::vec3( 1.5f, 0.0f, 0.51f),
           glm::vec3( 0.0f, 0.0f, 0.7f),
           glm::vec3(-0.3f, 0.0f, -2.3f),
           glm::vec3( 0.5f, 0.0f, -0.6f)
       };

      shader.use();
      shader.setInt("texture1", 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      while (!glfwWindowShouldClose(window)) {
          float currentFrame = glfwGetTime();
          deltaTime = currentFrame - lastFrame;
          lastFrame = currentFrame;
          camera.Position.x = 5.0 * sin(currentFrame);
          camera.Position.z = 5.0 * cos(currentFrame);
          camera.Front.x = -camera.Position.x;
          camera.Front.z = -camera.Position.z;
          camera.Front.y = -camera.Position.y;
          
          processInput(window);
          std::map<float, glm::vec3> sorted;
          for (unsigned int i = 0; i < windows.size(); i++) {
              float distance = glm::length(camera.Position - windows[i]);
              sorted[distance] = windows[i];
          }

          glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
          

          shader.use();
          glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
          glm::mat4 view = camera.GetViewMatrix();
          glm::mat4 model = glm::mat4(1.0f);
          shader.setMat4("projection", projection);
          shader.setMat4("view", view);

          glBindVertexArray(cubeVAO);
          glActiveTexture(GL_TEXTURE0);
          glBindTexture(GL_TEXTURE_2D, cubeTexture);
          model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
          shader.setMat4("model", model);
          glDrawArrays(GL_TRIANGLES, 0, 36);
          model = glm::mat4(1.0f);
          model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
          shader.setMat4("model", model);
          glDrawArrays(GL_TRIANGLES, 0, 36);

          glBindVertexArray(planeVAO);
          glBindTexture(GL_TEXTURE_2D, floorTexture);
          model = glm::mat4(1.0f);
          shader.setMat4("model", model);
          glDrawArrays(GL_TRIANGLES, 0, 6);
          
          glBindVertexArray(transparentVAO);
          glBindTexture(GL_TEXTURE_2D, windowTexture);
          for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it) {
              model = glm::mat4(1.0f);
              model = glm::translate(model, it->second);
              shader.setMat4("model", model);
              glDrawArrays(GL_TRIANGLES, 0, 6);
          }
          glfwSwapBuffers(window);
          glfwPollEvents();
      }
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
      glDeleteVertexArrays(1, &cubeVAO);
      glDeleteVertexArrays(1, &planeVAO);
      glDeleteBuffers(1, &cubeVBO);
      glDeleteBuffers(1, &planeVBO);

      glfwTerminate();
      return 0;
}


void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
