//
//  ShaderStreamInterface.h
//  Shaders
//
//  Created by 陈学明 on 2021/7/2.
//

#ifndef ShaderStreamInterface_h
#define ShaderStreamInterface_h

#define _SHADER_SRC(...) #__VA_ARGS__
#define SHADER_SRC(...) _SHADER_SRC(__VA_ARGS__)

class ShaderStreamInterface {
public:
    ~ShaderStreamInterface(){}
    virtual char *getVertexCode() = 0;
    
    virtual char *getFragmentCode() = 0;
protected:
    char *vertextCode;
    char *fragmentCode;
};

#endif /* ShaderStreamInterface_h */
