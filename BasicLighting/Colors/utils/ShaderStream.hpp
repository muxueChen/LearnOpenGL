//
//  ShaderStream.hpp
//  Shaders
//
//  Created by 陈学明 on 2021/7/2.
//

#ifndef ShaderStream_hpp
#define ShaderStream_hpp

#include <stdio.h>
#include "ShaderStreamInterface.h"
/** 章节案例 */
class ShaderStream: public ShaderStreamInterface {
public:
    ShaderStream();
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
#endif /* ShaderStream_hpp */
