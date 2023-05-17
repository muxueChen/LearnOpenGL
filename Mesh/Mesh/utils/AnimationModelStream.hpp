//
//  AnimationModelStream.hpp
//  Mesh
//
//  Created by 陈学明 on 2023/5/17.
//

#ifndef AnimationModelStream_hpp
#define AnimationModelStream_hpp

#include <stdio.h>
#include "ShaderStreamInterface.h"
/** 章节案例 */
class AnimationModelStream: public ShaderStreamInterface {
public:
    AnimationModelStream();
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
#endif /* AnimationModelStream_hpp */
