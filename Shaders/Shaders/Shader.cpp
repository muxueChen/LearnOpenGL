//
//  Shader.cpp
//  Shaders
//
//  Created by 陈学明 on 2021/7/1.
//

#include "Shader.hpp"

Shader:: Shader(ShaderStreamInterface *shaderStream) {
    // 1. 从文件路径中获取顶点/片段着色器
    const char* vShaderCode = shaderStream->getVertexCode();
    const char* fShaderCode = shaderStream->getFragmentCode();
    
    // 2. 编译着色器
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // 顶点着色器
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // 打印编译错误（如果有的话）
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // 片段着色器也类似
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    
    // 着色器程序
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // 打印连接错误（如果有的话）
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader * Shader:: use() {
    glUseProgram(ID);
    return this;
}

Shader * Shader:: setBool(const std::string &name, bool value) {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    return this;
}

Shader * Shader:: setInt(const std::string &name, int value) {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    return this;
}

Shader * Shader:: setFloat(const std::string &name, float value) {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    return this;
}

Shader * Shader:: setColor(const std::string &name, float red, float green, float blue, float alpha) {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), red, green, blue, alpha);
    return this;
}
