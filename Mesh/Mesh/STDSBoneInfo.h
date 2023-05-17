//
//  STDSBoneInfo.h
//  STDSRenderSDKIOS
//
//  Created by 陈学明 on 2023/5/17.
//

#ifndef STDSBoneInfo_h
#define STDSBoneInfo_h

#include<glm/glm.hpp>
namespace STDSSence {
struct BoneInfo {
    /*id is index in finalBoneMatrices*/
    int id;

    /*offset matrix transforms vertex from model space to bone space*/
    glm::mat4 offset;

};
}
#endif /* STDSBoneInfo_h */
