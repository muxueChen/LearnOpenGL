//
//  LamplightShader.hpp
//  Colors
//
//  Created by 陈学明 on 2021/10/12.
//

#ifndef LamplightShader_hpp
#define LamplightShader_hpp

#include <stdio.h>
#include "ShaderStreamInterface.h"

class LamplightShader: public ShaderStreamInterface {
public:
    LamplightShader();
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
#endif /* LamplightShader_hpp */
