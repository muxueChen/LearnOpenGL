//
//  ShaderStream2.hpp
//  Texture
//
//  Created by 陈学明 on 2021/7/11.
//

#ifndef ShaderStream2_hpp
#define ShaderStream2_hpp
#include <stdio.h>
#include "ShaderStreamInterface.h"

/** 尝试用不同的纹理环绕方式 */
class ShaderStream2: public ShaderStreamInterface {
public:
    ShaderStream2();
    char *getVertexCode() {
        return vertextCode;
    }
    char *getFragmentCode() {
        return fragmentCode;
    }
protected:
    char *vertexStream();
    char *fragmentStream();
};
#endif /* ShaderStream2_hpp */
