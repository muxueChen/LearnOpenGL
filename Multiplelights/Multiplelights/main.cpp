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


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;

Camera camera(glm::vec3(1.0f, 1.0f, 5.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 lightPos(1.2f, 0.0f, 2.0f);

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
    Shader lightingShader = Shader(shaderCode);
    
    LamplightShader *shaderCode1 = new LamplightShader();
    Shader lightCubeShader = Shader(shaderCode1);
    
    
    float vertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
        };
        // positions all containers
        glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
        };
        // first, configure the cube's VAO (and VBO)
        unsigned int VBO, cubeVAO;
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindVertexArray(cubeVAO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
        unsigned int lightCubeVAO;
        glGenVertexArrays(1, &lightCubeVAO);
        glBindVertexArray(lightCubeVAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // note that we update the lamp's position attribute's stride to reflect the updated buffer data
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // load textures (we now use a utility function to keep the code more organized)
        // -----------------------------------------------------------------------------
    unsigned int diffuseMap = UFImage("/Users/chenxueming/Desktop/LearnOpenGL/LightCasters/LightCasters/container2.png").glTexture();
    unsigned int specularMap = UFImage("/Users/chenxueming/Desktop/LearnOpenGL/LightCasters/LightCasters/container2_specular.png").glTexture();

        // shader configuration
        // --------------------
        lightingShader.use();
        lightingShader.setInt("material.diffuse", 0);
        lightingShader.setInt("material.specular", 1);

        glm::vec3 pointLightPositions[] = {
            glm::vec3( 0.7f,  0.2f,  2.0f),
            glm::vec3( 2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3( 0.0f,  0.0f, -3.0f)
        };
        glEnable(GL_DEPTH_TEST);
        while (!glfwWindowShouldClose(window)) {
                    float currentFrame = glfwGetTime();
                    deltaTime = currentFrame - lastFrame;
                    lastFrame = currentFrame;

                    processInput(window);
                
                    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    camera.Position.x = 8.0 *sin(glfwGetTime());
                    camera.Position.z = 8.0 *cos(glfwGetTime());
                    camera.Front = glm::vec3(-camera.Position.x, -camera.Position.y, -camera.Position.z);
                    lightingShader.use();
                    lightingShader.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
                    lightingShader.setFloat("material.shininess", 32.0f);

                    // directional light
                    lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
                    lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
                    lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
                    lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
                    // point light 1
                    lightingShader.setVec3("pointLights[0].position", pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
                    lightingShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
                    lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
                    lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
                    lightingShader.setFloat("pointLights[0].constant", 1.0f);
                    lightingShader.setFloat("pointLights[0].linear", 0.09);
                    lightingShader.setFloat("pointLights[0].quadratic", 0.032);
                    // point light 2
                    lightingShader.setVec3("pointLights[1].position", pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
                    lightingShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
                    lightingShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
                    lightingShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
                    lightingShader.setFloat("pointLights[1].constant", 1.0f);
                    lightingShader.setFloat("pointLights[1].linear", 0.09);
                    lightingShader.setFloat("pointLights[1].quadratic", 0.032);
                    // point light 3
                    lightingShader.setVec3("pointLights[2].position", pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
                    lightingShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
                    lightingShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
                    lightingShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
                    lightingShader.setFloat("pointLights[2].constant", 1.0f);
                    lightingShader.setFloat("pointLights[2].linear", 0.09);
                    lightingShader.setFloat("pointLights[2].quadratic", 0.032);
                    // point light 4
                    lightingShader.setVec3("pointLights[3].position", pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
                    lightingShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
                    lightingShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
                    lightingShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
                    lightingShader.setFloat("pointLights[3].constant", 1.0f);
                    lightingShader.setFloat("pointLights[3].linear", 0.09);
                    lightingShader.setFloat("pointLights[3].quadratic", 0.032);
                    // spotLight
                    lightingShader.setVec3("spotLight.position", camera.Position.x, camera.Position.y, camera.Position.z);
                    lightingShader.setVec3("spotLight.direction", camera.Front.x, camera.Front.y, camera.Front.z);
                    lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
                    lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
                    lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
                    lightingShader.setFloat("spotLight.constant", 1.0f);
                    lightingShader.setFloat("spotLight.linear", 0.09);
                    lightingShader.setFloat("spotLight.quadratic", 0.032);
                    lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
                    lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

                    // view/projection transformations
                    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
                    glm::mat4 view = camera.GetViewMatrix();
                    lightingShader.setMat4("projection", projection);
                    lightingShader.setMat4("view", view);

                    // world transformation
                    glm::mat4 model = glm::mat4(1.0f);
                    lightingShader.setMat4("model", model);

                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, diffuseMap);
                    glActiveTexture(GL_TEXTURE1);
                    glBindTexture(GL_TEXTURE_2D, specularMap);

                    glBindVertexArray(cubeVAO);
                    for (unsigned int i = 0; i < 10; i++) {
                        glm::mat4 model = glm::mat4(1.0f);
                        model = glm::translate(model, cubePositions[i]);
                        float angle = 20.0f * i;
                        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                        lightingShader.setMat4("model", model);

                        glDrawArrays(GL_TRIANGLES, 0, 36);
                    }

                     // also draw the lamp object(s)
                     lightCubeShader.use();
                     lightCubeShader.setMat4("projection", projection);
                     lightCubeShader.setMat4("view", view);
                
                     // we now draw as many light bulbs as we have point lights.
                     glBindVertexArray(lightCubeVAO);
                     for (unsigned int i = 0; i < 4; i++)
                     {
                         model = glm::mat4(1.0f);
                         model = glm::translate(model, pointLightPositions[i]);
                         model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
                         lightCubeShader.setMat4("model", model);
                         glDrawArrays(GL_TRIANGLES, 0, 36);
                     }


                    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
                    // -------------------------------------------------------------------------------
                    glfwSwapBuffers(window);
                    glfwPollEvents();

        }

        glDisable(GL_DEPTH_TEST);
        // optional: de-allocate all resources once they've outlived their purpose:
        // ------------------------------------------------------------------------
        glDeleteVertexArrays(1, &cubeVAO);
        glDeleteVertexArrays(1, &lightCubeVAO);
        glDeleteBuffers(1, &VBO);

        // glfw: terminate, clearing all previously allocated GLFW resources.
        // ------------------------------------------------------------------
        glfwTerminate();
        return 0;
}


void processInput(GLFWwindow *window)
{
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
