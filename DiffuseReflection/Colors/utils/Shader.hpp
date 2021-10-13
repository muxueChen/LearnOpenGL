//
//  Shader.hpp
//  Shaders
//
//  Created by 陈学明 on 2021/7/1.
//

#ifndef Shader_hpp
#define Shader_hpp

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <iostream>

#include "ShaderStreamInterface.h"
/** 着色器类 */
class Shader {
    
public:
    // 程序ID
    unsigned int ID;
    // 构造器读取并构建着色器
    Shader(ShaderStreamInterface *shaderStream);
    // 使用/激活程序
    Shader * use();
    Shader * unuse();
    // uniform工具函数
    Shader * setBool(const std::string &name, bool value);
    Shader * setInt(const std::string &name, int value);
    Shader * setFloat(const std::string &name, float value);
    Shader * setVec4(const std::string &name, float red, float green, float blue, float alpha);
    Shader * setVec2(const std::string &name, float f1, float f2);
    Shader * setVec3(const std::string &name, float f1, float f2, float f3);
    Shader * setMat2(const std::string &name, const glm::mat2 &value);
    Shader * setMat3(const std::string &name, const glm::mat3 &value);
    Shader * setMat4(const std::string &name, const glm::mat4 &value);
};
#endif /* Shader_hpp */
