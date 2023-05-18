//
//  animator.h
//  Mesh
//
//  Created by 陈学明 on 2023/5/17.
//

#ifndef animator_h
#define animator_h

#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include "animation.h"
#include "bone.h"


class Animator {
public:
    Animator(vector<shared_ptr<Animation>> animations) {
        m_CurrentTime = 0.0;
        m_Animations = animations;
        m_FinalBoneMatrices.reserve(100);
        for (int i = 0; i < 100; i++) {
            m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
        }
    }
    Animator(shared_ptr<Animation> animation) {
        m_CurrentTime = 0.0;
        m_CurrentAnimation = animation;

        m_FinalBoneMatrices.reserve(100);

        for (int i = 0; i < 100; i++) {
            m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
        }
    }

    void UpdateAnimation(float dt)
    {
        m_DeltaTime = dt;
        if (m_CurrentAnimation)
        {
            m_CurrentAnimation->m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;
            m_CurrentAnimation->m_CurrentTime = fmod(m_CurrentAnimation->m_CurrentTime, m_CurrentAnimation->GetDuration());
            CalculateBoneTransform(m_CurrentAnimation, &m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
        }
        for (auto item:m_Animations) {
            item->m_CurrentTime += item->GetTicksPerSecond() * dt;
            item->m_CurrentTime = fmod(item->m_CurrentTime, item->GetDuration());
            CalculateBoneTransform(item, &item->GetRootNode(), glm::mat4(1.0f));
        }
    }

    void CalculateBoneTransform(shared_ptr<Animation> animation, const AssimpNodeData* node, glm::mat4 parentTransform) {
        
        std::string nodeName = node->name;
        glm::mat4 nodeTransform = node->transformation;

        Bone* Bone = animation->FindBone(nodeName);

        if (Bone) {
            Bone->Update(animation->m_CurrentTime);
            nodeTransform = Bone->GetLocalTransform();
        }

        glm::mat4 globalTransformation = parentTransform * nodeTransform;

        auto boneInfoMap = animation->GetBoneIDMap();
        if (boneInfoMap.find(nodeName) != boneInfoMap.end()) {
            int index = boneInfoMap[nodeName].id;
            glm::mat4 offset = boneInfoMap[nodeName].offset;
            m_FinalBoneMatrices[index] = globalTransformation * offset;
        }

        for (int i = 0; i < node->childrenCount; i++) {
            CalculateBoneTransform(animation, &node->children[i], globalTransformation);
        }
            
    }

    std::vector<glm::mat4> GetFinalBoneMatrices()
    {
        return m_FinalBoneMatrices;
    }

private:
    std::vector<glm::mat4> m_FinalBoneMatrices;
    vector<shared_ptr<Animation>> m_Animations;
    shared_ptr<Animation> m_CurrentAnimation;
    float m_CurrentTime;
    float m_DeltaTime;

};
#endif /* animator_h */
