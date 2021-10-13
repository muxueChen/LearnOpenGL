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
// 材质
struct Material {
    vec3 ambient;// 环境光
    vec3 diffuse;//漫反射光
    vec3 specular;//镜面反射光
    float shininess;//反光度
};
                      
struct Light {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

uniform Light light;

uniform Material material;
                      
in vec3 Normal;// 法向量
in vec3 FragPos;//片段位置

uniform vec3 lightPos;// 光源位置
uniform vec3 viewPos;// 摄像机的位置
uniform vec3 lightColor;// 环境光的颜色
uniform vec3 objectColor;// 物体的颜色

void main() {
    // 环境光
    vec3 ambient = light.ambient*lightColor * material.ambient;

    // 漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse*lightColor * (diff * material.diffuse);

    // 镜面光
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular*lightColor * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    
    FragColor = vec4(result, 1.0);
}
);
}
