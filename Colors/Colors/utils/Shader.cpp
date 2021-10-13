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
    // 打印连接错误（如果有的话）
    glGetProgramiv(ID, GL_VALIDATE_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
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

Shader * Shader:: unuse() {
    glUseProgram(0);
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

Shader * Shader:: setVec4(const std::string &name, float f1, float f2, float f3, float f4) {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), f1, f2, f3, f4);
    return this;
}
Shader * Shader:: setVec2(const std::string &name, float f1, float f2) {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), f1, f2);
    return this;
}

Shader * Shader:: setVec3(const std::string &name, float f1, float f2, float f3) {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), f1, f2, f3);
    return this;
}

Shader * Shader:: setMat2(const std::string &name, const glm::mat2 &value) {
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,  &value[0][0]);
    return this;
}

Shader * Shader:: setMat3(const std::string &name,  const glm::mat3 &value) {
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,  &value[0][0]);
    return this;
}

Shader * Shader:: setMat4(const std::string &name, const glm::mat4 &value) {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
    return this;
}
