//
//  WorkShderStream1.hpp
//  Shaders
//
//  Created by 陈学明 on 2021/7/3.
//

#ifndef WorkShderStream1_hpp
#define WorkShderStream1_hpp

#include <stdio.h>
#include "ShaderStreamInterface.h"

/** 作业1 */
class WorkShderStream1: public ShaderStreamInterface {
    WorkShderStream1();
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

#endif /* WorkShderStream1_hpp */
