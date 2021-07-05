//
//  WorkShderStream2.cpp
//  Shaders
//
//  Created by 陈学明 on 2021/7/3.
//

#include "WorkShderStream2.hpp"

WorkShderStream2:: WorkShderStream2() {
    // 顶点着色器
    vertextCode = vertexStream();
    // 片段着色器代码
    fragmentCode = fragmentStream();
}


char * WorkShderStream2:: vertexStream() {
    return SHADER_SRC(
                      \#version 330 core\n
                      layout (location = 0) in vec3 aPos;
                      layout (location = 1) in vec3 aColor;
                      uniform float offsetx;
                      out vec3 ourColor;

                      void main() {
                          gl_Position = vec4(aPos.x + offsetx, aPos.y, aPos.z, 1.0);
                          ourColor = aColor;
                      }
            );
}

char * WorkShderStream2:: fragmentStream() {
    return SHADER_SRC(
                      \#version 330 core\n
                      out vec4 FragColor;
                      in vec3 ourColor;
                      void main() {
                          FragColor = vec4(ourColor, 1.0);
                      }
    );
}
