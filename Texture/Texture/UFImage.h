//
//  UFImageDecode.hpp
//  libpng
//
//  Created by UFOTO on 2021/5/13.
//

#include <stdio.h>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"
#include <stdio.h>
#define GLEW_STATIC
#include <GL/glew.h>

using namespace std;

enum UFPixelSpace {
    UFPixelSpaceGrayscale = 1, // 灰度图
    UFPixelSpaceGrayscaleAndAlpha, // 灰度加透明度
    UFPixelSpaceRGB, // rgb 颜色空间
    UFPixelSpaceRGBA, // rgba 颜色空间
};

class UFImage {
public:
    unsigned char *data{nullptr}; // 图片数据
    UFPixelSpace format; // 图像色彩空间格式
    int width; // 图像宽度
    int height; // 图像高度
    UFImage(std::string path) {
        this->path = path;
        textureID = 0;
        decode();
    }
    
    /** 图片转纹理 */
    GLuint glTexture() {
        if (textureID) {
            return textureID;
        }
        // 函数首先需要输入生成纹理的数量，然后把它们储存在第二个参数的unsigned int数组中
        glGenTextures(1, &textureID);
        // 绑定纹理，让之后任何的纹理指令都可以配置当前绑定的纹理：
        glBindTexture(GL_TEXTURE_2D, textureID);
        // 纹理过滤器
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)GL_CLAMP_TO_EDGE);
        
        // 图片纹理通过 glTexImage2D 来生成
        if (format == UFPixelSpaceRGBA) {
            glTexImage2D(GL_TEXTURE_2D, 0, (GLint)GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        } else if (format == UFPixelSpaceRGB) {
            glTexImage2D(GL_TEXTURE_2D, 0, (GLint)GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        } else if (format == UFPixelSpaceGrayscale) {
            glTexImage2D(GL_TEXTURE_2D, 0, (GLint)GL_RG, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, data);
        } else {
            glTexImage2D(GL_TEXTURE_2D, 0, (GLint)GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
        }
        return textureID;
    }
    
private:
    GLuint textureID;
    string path;
    void decode() {
        int iw, ih, n;
        data = stbi_load(path.c_str(), &iw, &ih, &n, 0);
        format = static_cast<UFPixelSpace>(n);
        width = iw;
        height = ih;
    }
    
    void destroy() {
        if (textureID) {
            glDeleteTextures(1, &textureID);
            textureID = 0;
        }
        if (data) {
            stbi_image_free(data);
            data = NULL;
        }
    }
};
