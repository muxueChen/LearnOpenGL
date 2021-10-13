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
        layout (location = 1) in vec3 aColor;
        layout (location = 2) in vec2 aTexCoord;

        out vec3 ourColor;
        out vec2 TexCoord;

        void main() {
            gl_Position = vec4(aPos, 1.0);
            ourColor = aColor;
            TexCoord = aTexCoord;
        }
    );
}

char * ShaderStream:: fragmentStream() {
    return SHADER_SRC(
        \#version 330 core\n
        out vec4 FragColor;
        in vec3 ourColor;
        in vec2 TexCoord;
        uniform sampler2D texture1;
        uniform sampler2D texture2;

        void main() {
//            FragColor = vec4(ourColor, 1.0);
//            FragColor = texture(ourTexture, TexCoord);
//            FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
            // 混合纹理
//            FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
        
            // 修改笑脸看的方向
        vec2 faceCoord = vec2(1.0 - TexCoord.x, TexCoord.y);
        FragColor = mix(texture(texture1, TexCoord), texture(texture2, faceCoord), 0.2);
        }
    );
}
