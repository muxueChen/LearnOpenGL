//
//  WorkShderStream3.hpp
//  Shaders
//
//  Created by 陈学明 on 2021/7/3.
//

#ifndef WorkShderStream3_hpp
#define WorkShderStream3_hpp

#include <stdio.h>
#include "ShaderStreamInterface.h"

/** 作业3 */
class WorkShderStream3: public ShaderStreamInterface {
    WorkShderStream3();
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
#endif /* WorkShderStream3_hpp */
