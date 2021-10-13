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
      layout (location = 1) in vec3 aNormal;

      out vec3 FragPos;
      out vec3 Normal;

      uniform mat4 model;
      uniform mat4 view;
      uniform mat4 projection;

      void main()
      {
          // 得到世界坐标的位置
          FragPos = vec3(model * vec4(aPos, 1.0));
          // 计算法向量
          Normal = mat3(transpose(inverse(model))) * aNormal;
          // 最终渲染的顶点坐标
          gl_Position = projection * view * vec4(FragPos, 1.0);
      }
    );
}

char * ShaderStream:: fragmentStream() {
    return SHADER_SRC(
\#version 330 core\n
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;// 环境光的颜色
uniform vec3 objectColor;// 物体的颜色

void main()
{
  // 环境光照的颜色
  float ambientStrength = 0.1;
  vec3 ambient = ambientStrength * lightColor;
    
  // 漫反射光照
    // 归一化法向量
  vec3 norm = normalize(Normal);
    // 归一化后的光线入射向量
  vec3 lightDir = normalize(lightPos - FragPos);
    // 得到片段法向量与光线向量的余弦值
  float diff = max(dot(norm, lightDir), 0.0);
    // 漫反射光的颜色
  vec3 diffuse = diff * lightColor;
  
  // 镜面反射光最大强度
  float specularStrength = 0.5;
    // 视线归一化向量
  vec3 viewDir = normalize(viewPos - FragPos);
    // 计算得出反射归一化向量
  vec3 reflectDir = reflect(-lightDir, norm);
    // 片段的镜面反射分量
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // 镜面反色的颜色
  vec3 specular = specularStrength * spec * lightColor;
      
  vec3 result = (ambient + diffuse + specular) * objectColor;
  FragColor = vec4(result, 1.0);
}
);
}
