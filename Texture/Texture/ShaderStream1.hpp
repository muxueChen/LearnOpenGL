//
//  ShaderStream1.hpp
//  Texture
//
//  Created by 陈学明 on 2021/7/11.
//

#ifndef ShaderStream1_hpp
#define ShaderStream1_hpp
#include <stdio.h>
#include "ShaderStreamInterface.h"

/** 修改笑脸看的方向 */
class ShaderStream1: public ShaderStreamInterface {
public:
    ShaderStream1();
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
#endif /* ShaderStream1_hpp */
