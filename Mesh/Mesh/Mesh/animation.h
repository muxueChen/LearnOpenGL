//
//  animation.h
//  Mesh
//
//  Created by 陈学明 on 2023/5/17.
//

#ifndef animation_h
#define animation_h
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include "bone.h"
#include <functional>
#include "animdata.h"
#include "Model.h"
#include <memory>

struct AssimpNodeData
{
    glm::mat4 transformation;
    std::string name;
    int childrenCount{0};
    std::vector<AssimpNodeData> children;
};

class Animation
{
public:
    
    static vector<shared_ptr<Animation>> parseAnimation(const std::string& animationPath, Model* model) {
        vector<shared_ptr<Animation>> list;
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
        assert(scene && scene->mRootNode);
        if (scene->mNumAnimations <= 0) {
            return list;
        }
        for (int i = 0; i < scene->mNumAnimations; i ++) {
            auto animation = scene->mAnimations[i];
            auto anima = make_shared<Animation>(scene, animation, model);
            list.push_back(anima);
        }
        return list;
    }
    
    float m_CurrentTime{0.0};
    Animation() = default;

    Animation(const std::string& animationPath, Model* model)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
        assert(scene && scene->mRootNode);
        if (scene->mNumAnimations <= 0) {
            return;
        }
        for (int i = 0; i < scene->mNumAnimations; i ++) {
            auto animation = scene->mAnimations[i];
            m_Duration = animation->mDuration;
            m_TicksPerSecond = animation->mTicksPerSecond;
            aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
            globalTransformation = globalTransformation.Inverse();
            ReadHierarchyData(m_RootNode, scene->mRootNode);
            ReadMissingBones(animation, *model);
        }
    }
    
    Animation(const aiScene* scene, aiAnimation *animation, Model* model) {
        m_Duration = animation->mDuration;
        m_TicksPerSecond = animation->mTicksPerSecond;
        aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
        globalTransformation = globalTransformation.Inverse();
        ReadHierarchyData(m_RootNode, scene->mRootNode);
        ReadMissingBones(animation, *model);
    }
    
    ~Animation() {}

    Bone* FindBone(const std::string& name)
    {
        auto iter = std::find_if(m_Bones.begin(), m_Bones.end(),
            [&](const Bone& Bone)
            {
                return Bone.GetBoneName() == name;
            }
        );
        if (iter == m_Bones.end()) return nullptr;
        else return &(*iter);
    }

    
    inline float GetTicksPerSecond() { return m_TicksPerSecond; }
    inline float GetDuration() { return m_Duration;}
    inline const AssimpNodeData& GetRootNode() { return m_RootNode; }
    inline const std::map<std::string,BoneInfo>& GetBoneIDMap()
    {
        return m_BoneInfoMap;
    }

private:


    void ReadMissingBones(const aiAnimation* animation, Model& model)
    {
        int size = animation->mNumChannels;

        auto& boneInfoMap = model.GetBoneInfoMap();//getting m_BoneInfoMap from Model class
        int& boneCount = model.GetBoneCount(); //getting the m_BoneCounter from Model class

        //reading channels(bones engaged in an animation and their keyframes)
        for (int i = 0; i < size; i++)
        {
            auto channel = animation->mChannels[i];
            std::string boneName = channel->mNodeName.data;

            if (boneInfoMap.find(boneName) == boneInfoMap.end())
            {
                boneInfoMap[boneName].id = boneCount;
                boneCount++;
            }
            m_Bones.push_back(Bone(channel->mNodeName.data,
                boneInfoMap[channel->mNodeName.data].id, channel));
        }

        m_BoneInfoMap = boneInfoMap;
    }

    void ReadHierarchyData(AssimpNodeData& dest, const aiNode* src)
    {
        assert(src);

        dest.name = src->mName.data;
        dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);
        dest.childrenCount = src->mNumChildren;

        for (int i = 0; i < src->mNumChildren; i++)
        {
            AssimpNodeData newData;
            ReadHierarchyData(newData, src->mChildren[i]);
            dest.children.push_back(newData);
        }
    }
    float m_Duration{0.0};
    int m_TicksPerSecond{0};
    std::vector<Bone> m_Bones;
    AssimpNodeData m_RootNode;
    std::map<std::string, BoneInfo> m_BoneInfoMap;
};

#endif /* animation_h */
