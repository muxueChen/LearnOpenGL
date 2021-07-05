//
//  WorkShderStream2.hpp
//  Shaders
//
//  Created by 陈学明 on 2021/7/3.
//

#ifndef WorkShderStream2_hpp
#define WorkShderStream2_hpp

#include <stdio.h>
#include "ShaderStreamInterface.h"

/** 作业2 */
class WorkShderStream2: public ShaderStreamInterface {
    WorkShderStream2();
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

#endif /* WorkShderStream2_hpp */
