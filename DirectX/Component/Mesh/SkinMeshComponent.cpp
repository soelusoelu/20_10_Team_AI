#include "SkinMeshComponent.h"
#include "MeshComponent.h"
#include "MeshShader.h"
#include "../../DebugLayer/Debug.h"
#include "../../Mesh/IAnimation.h"
#include "../../System/Shader/ConstantBuffers.h"
#include <cassert>

SkinMeshComponent::SkinMeshComponent(GameObject& gameObject)
    : Component(gameObject)
    , mAnimation(nullptr)
    , mMeshShader(nullptr)
    , mCurrentMotionNo(0)
    , mCurrentFrame(0)
{
}

SkinMeshComponent::~SkinMeshComponent() = default;

void SkinMeshComponent::start() {
    mAnimation = getComponent<MeshComponent>()->getAnimation();
    mMeshShader = getComponent<MeshShader>();

    mCurrentBones.resize(mAnimation->getBoneCount());
}

void SkinMeshComponent::update() {
    const auto& motion = mAnimation->getMotion(mCurrentMotionNo);
    ++mCurrentFrame;
    if (mCurrentFrame >= motion.numFrame) {
        mCurrentFrame = 0;
    }

    //シェーダーにボーンのデータを渡す
    for (size_t i = 0; i < mAnimation->getBoneCount(); ++i) {
        mCurrentBones[i] = mAnimation->getBone(i).offsetMat * motion.frameMat[i][mCurrentFrame];
    }
    mMeshShader->setTransferData(mCurrentBones.data(), sizeof(SkinMeshConstantBuffer), 2);
}

void SkinMeshComponent::setMotionName(const std::string& name, unsigned motionNo) {
    assert(motionNo < mAnimation->getMotionCount());
    mAnimation->setMotionName(name, motionNo);
}

void SkinMeshComponent::changeMotion(unsigned motionNo) {
    assert(motionNo < mAnimation->getMotionCount());
    mCurrentMotionNo = motionNo;
    mCurrentFrame = 0;
}

void SkinMeshComponent::changeMotion(const std::string& motionName) {
    for (size_t i = 0; i < mAnimation->getMotionCount(); ++i) {
        if (mAnimation->getMotion(i).name == motionName) {
            mCurrentMotionNo = i;
            mCurrentFrame = 0;
            return;
        }
    }

    Debug::logError("Not found motion name[" + motionName + "].");
}

const Motion& SkinMeshComponent::getCurrentMotion() const {
    return mAnimation->getMotion(mCurrentMotionNo);
}

int SkinMeshComponent::getCurrentMotionFrame() const {
    return mCurrentFrame;
}

const std::vector<Matrix4>& SkinMeshComponent::getBoneCurrentFrameMatrix() const {
    return mCurrentBones;
}
