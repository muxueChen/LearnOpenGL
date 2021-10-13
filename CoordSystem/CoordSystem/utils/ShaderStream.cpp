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
        layout (location = 1) in vec2 aTexCoord;

        out vec2 TexCoord;
      uniform mat4 model;
      uniform mat4 view;
      uniform mat4 projection;
                      
        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
            TexCoord = vec2(aTexCoord.x, aTexCoord.y);
        }
    );
}

char * ShaderStream:: fragmentStream() {
    return SHADER_SRC(
        \#version 330 core\n
        out vec4 FragColor;
        in vec2 TexCoord;
        uniform sampler2D texture1;
        uniform sampler2D texture2;

        void main() {
            // 修改笑脸看的方向
            vec2 faceCoord = TexCoord;
            FragColor = mix(texture(texture1, TexCoord), texture(texture2, faceCoord), 0.2);
        }
    );
}
