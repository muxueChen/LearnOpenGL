//
//  animdata.h
//  Mesh
//
//  Created by 陈学明 on 2023/5/17.
//

#ifndef animdata_h
#define animdata_h

#include<glm/glm.hpp>

struct BoneInfo
{
    /*id is index in finalBoneMatrices*/
    int id;

    /*offset matrix transforms vertex from model space to bone space*/
    glm::mat4 offset;

};
#endif /* animdata_h */
