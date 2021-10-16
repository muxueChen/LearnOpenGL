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
                      layout (location = 2) in vec2 aTexCoords;

                      out vec3 FragPos;
                      out vec3 Normal;
                      out vec2 TexCoords;

                      uniform mat4 model;
                      uniform mat4 view;
                      uniform mat4 projection;

                      void main()
                      {
                          FragPos = vec3(model * vec4(aPos, 1.0));
                          Normal = mat3(transpose(inverse(model))) * aNormal;
                          TexCoords = aTexCoords;
                          
                          gl_Position = projection * view * vec4(FragPos, 1.0);
                      }
    );
}

char * ShaderStream:: fragmentStream() {
    return SHADER_SRC(
\#version 330 core\n
  out vec4 FragColor;

  struct Material {
      sampler2D diffuse;
      sampler2D specular;
      float shininess;
  };

  struct DirLight {
      vec3 direction;
      
      vec3 ambient;
      vec3 diffuse;
      vec3 specular;
  };

  struct PointLight {
      vec3 position;
      
      float constant;
      float linear;
      float quadratic;
      
      vec3 ambient;
      vec3 diffuse;
      vec3 specular;
  };

  struct SpotLight {
      vec3 position;
      vec3 direction;
      float cutOff;
      float outerCutOff;
    
      float constant;
      float linear;
      float quadratic;
    
      vec3 ambient;
      vec3 diffuse;
      vec3 specular;
  };

  #define NR_POINT_LIGHTS 4

  in vec3 FragPos;
  in vec3 Normal;
  in vec2 TexCoords;

  uniform vec3 viewPos;
  uniform DirLight dirLight;
  uniform PointLight pointLights[NR_POINT_LIGHTS];
  uniform SpotLight spotLight;
  uniform Material material;

  // function prototypes
  vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
  vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
  vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

  void main() {
      vec3 norm = normalize(Normal);
      vec3 viewDir = normalize(viewPos - FragPos);
      
      vec3 result = CalcDirLight(dirLight, norm, viewDir);
      for(int i = 0; i < NR_POINT_LIGHTS; i++) {
          result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
      }
      result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
      
      FragColor = vec4(result, 1.0);
  }

  vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
      vec3 lightDir = normalize(-light.direction);
      // diffuse shading
      float diff = max(dot(normal, lightDir), 0.0);
      // specular shading
      vec3 reflectDir = reflect(-lightDir, normal);
      float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
      // combine results
      vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
      vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
      vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
      return (ambient + diffuse + specular);
  }


  vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
      vec3 lightDir = normalize(light.position - fragPos);
      // diffuse shading
      float diff = max(dot(normal, lightDir), 0.0);
      // specular shading
      vec3 reflectDir = reflect(-lightDir, normal);
      float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
      // attenuation
      float distance = length(light.position - fragPos);
      float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
      // combine results
      vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
      vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
      vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
      ambient *= attenuation;
      diffuse *= attenuation;
      specular *= attenuation;
      return (ambient + diffuse + specular);
  }

  // calculates the color when using a spot light.
  vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
  {
      vec3 lightDir = normalize(light.position - fragPos);
      // diffuse shading
      float diff = max(dot(normal, lightDir), 0.0);
      // specular shading
      vec3 reflectDir = reflect(-lightDir, normal);
      float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
      // attenuation
      float distance = length(light.position - fragPos);
      float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
      // spotlight intensity
      float theta = dot(lightDir, normalize(-light.direction));
      float epsilon = light.cutOff - light.outerCutOff;
      float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
      // combine results
      vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
      vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
      vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
      ambient *= attenuation * intensity;
      diffuse *= attenuation * intensity;
      specular *= attenuation * intensity;
      return (ambient + diffuse + specular);
  }
);
}
