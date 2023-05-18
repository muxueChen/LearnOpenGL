//
//  AnimationModelStream.cpp
//  Mesh
//
//  Created by 陈学明 on 2023/5/17.
//

#include "AnimationModelStream.hpp"

AnimationModelStream:: AnimationModelStream() {
    // 顶点着色器
    vertextCode = vertexStream();
    // 片段着色器代码
    fragmentCode = fragmentStream();
}

char * AnimationModelStream:: vertexStream() {
    return SHADER_SRC(
\n#version 330 core\n

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 tex;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;
layout(location = 5) in ivec4 boneIds;
layout(location = 6) in vec4 weights;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

out vec2 TexCoords;

void main()
{
    vec4 totalPosition = vec4(0.0);
    float flag = 0.0;
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++) {
        if(boneIds[i] == -1)
            continue;
        if(boneIds[i] >=MAX_BONES)
        {
            totalPosition = vec4(pos,1.0f);
            break;
        }
        flag = 1.0;
        vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(pos,1.0f);
        totalPosition += localPosition * weights[i];
        vec3 localNormal = mat3(finalBonesMatrices[boneIds[i]]) * norm;
   }
    totalPosition = totalPosition * flag + vec4(pos,1.0f) * (1.0 - flag);
    mat4 viewModel = view * model;
    gl_Position =  projection * viewModel * totalPosition;
    TexCoords = tex;
}
                  );
}

char * AnimationModelStream:: fragmentStream() {
    return SHADER_SRC(
\n#version 330 core\n
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{
    FragColor = texture(texture_diffuse1, TexCoords);
    FragColor = vec4(TexCoords.y);
}
);
}
