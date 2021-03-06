﻿#include "SkinMeshComponent.h"
#include "MeshComponent.h"
#include "MeshShader.h"
#include "../../../Engine/DebugManager/DebugUtility/Debug.h"
#include "../../../System/Shader/ConstantBuffers.h"
#include <cassert>

SkinMeshComponent::SkinMeshComponent(GameObject& gameObject)
    : Component(gameObject)
    , mAnimation(nullptr)
    , mMeshShader(nullptr)
    , mCurrentMotionNo(0)
    , mCurrentFrame(0)
    , mIsTPose(false)
{
}

SkinMeshComponent::~SkinMeshComponent() = default;

void SkinMeshComponent::update() {
    if (mIsTPose) {
        mCurrentBones.assign(mCurrentBones.size(), Matrix4::identity);
    } else {
        const auto& motion = mAnimation->getMotion(mCurrentMotionNo);
        ++mCurrentFrame;
        if (mCurrentFrame >= motion.numFrame) {
            mCurrentFrame = 0;
        }

        //シェーダーにボーンのデータを渡す
        for (size_t i = 0; i < mAnimation->getBoneCount(); ++i) {
            mCurrentBones[i] = mAnimation->getBone(i).offsetMat * motion.frameMat[i][mCurrentFrame];
        }
    }

    mMeshShader->setTransferData(mCurrentBones.data(), sizeof(SkinMeshConstantBuffer), 3);
}

void SkinMeshComponent::setMotionName(const std::string& name, unsigned motionNo) {
    assert(motionNo < mAnimation->getMotionCount());
    mAnimation->setMotionName(name, motionNo);
}

void SkinMeshComponent::changeMotion(unsigned motionNo) {
    assert(motionNo < mAnimation->getMotionCount());
    mCurrentMotionNo = motionNo;
    mCurrentFrame = 0;
    mIsTPose = false;
}

void SkinMeshComponent::changeMotion(const std::string& motionName) {
    for (size_t i = 0; i < mAnimation->getMotionCount(); ++i) {
        if (mAnimation->getMotion(i).name == motionName) {
            mCurrentMotionNo = i;
            mCurrentFrame = 0;
            mIsTPose = false;
            return;
        }
    }

    Debug::logError("Not found motion name[" + motionName + "].");
}

void SkinMeshComponent::tPose() {
    mIsTPose = true;
}

int SkinMeshComponent::getMotionCount() const {
    return mAnimation->getMotionCount();
}

const Motion& SkinMeshComponent::getCurrentMotion() const {
    return mAnimation->getMotion(mCurrentMotionNo);
}

int SkinMeshComponent::getCurrentMotionNumber() const {
    return mCurrentMotionNo;
}

int SkinMeshComponent::getCurrentMotionFrame() const {
    return mCurrentFrame;
}

const std::vector<Matrix4>& SkinMeshComponent::getBoneCurrentFrameMatrix() const {
    return mCurrentBones;
}

void SkinMeshComponent::setValue(const std::shared_ptr<MeshShader>& meshShader, IAnimation* anim) {
    mAnimation = anim;
    mMeshShader = meshShader;

    mCurrentBones.resize(mAnimation->getBoneCount());
}
