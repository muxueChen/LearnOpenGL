//
//  ShaderStream.cpp
//  Shaders
//
//  Created by 陈学明 on 2021/7/2.
//

#include "ShaderStream.hpp"

ShaderStream:: ShaderStream() {
    // 顶点着色器
    vertextCode = vertexStream();
    // 片段着色器代码
    fragmentCode = fragmentStream();
}

char * ShaderStream:: vertexStream() {
    return SHADER_SRC(
    \#version 330 core\n
      layout (location = 0) in vec3 aPos;
      out vec3 TexCoords;
      uniform mat4 projection;
      uniform mat4 view;
      void main() {
          TexCoords = aPos;
          vec4 pos = projection * view * vec4(aPos, 1.0);
          gl_Position = pos.xyww;
      }
    );
}

char * ShaderStream:: fragmentStream() {
    return SHADER_SRC(
      \#version 330 core\n
      out vec4 FragColor;
      in vec3 TexCoords;
      uniform samplerCube skybox;
      void main() {
          FragColor = texture(skybox, TexCoords);
      }
);
}
