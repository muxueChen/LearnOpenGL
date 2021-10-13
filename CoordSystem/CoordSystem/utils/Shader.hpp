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
    // uniform工具函数
    Shader * setBool(const std::string &name, bool value);
    Shader * setInt(const std::string &name, int value);
    Shader * setFloat(const std::string &name, float value);
    Shader * setColor(const std::string &name, float red, float green, float blue, float alpha);
};
#endif /* Shader_hpp */
