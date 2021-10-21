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
  layout (location = 1) in vec2 aTexCoords;

  out vec2 TexCoords;

  uniform mat4 model;
  uniform mat4 view;
  uniform mat4 projection;

  void main() {
      TexCoords = aTexCoords;
      gl_Position = projection * view * model * vec4(aPos, 1.0);
  }
    );
}

char * ShaderStream:: fragmentStream() {
    return SHADER_SRC(
    \#version 330 core\n
    out vec4 FragColor;

    in vec2 TexCoords;

    uniform sampler2D texture1;

    void main() {
      vec4 texColor = texture(texture1, TexCoords);
      if(texColor.a < 0.1)
          discard;
      FragColor = texColor;
    }
);
}
