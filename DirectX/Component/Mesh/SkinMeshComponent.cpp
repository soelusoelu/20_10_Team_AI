#include "SkinMeshComponent.h"
#include "MeshComponent.h"
#include "MeshShader.h"
#include "../../DebugLayer/Debug.h"
#include "../../Mesh/IAnimation.h"
#include "../../System/Shader/ConstantBuffers.h"
#include <cassert>

SkinMeshComponent::SkinMeshComponent(GameObject& gameObject)
    : Component(gameObject)
    , mMesh(nullptr)
    , mMeshShader(nullptr)
    , mCurrentMotionNo(0)
    , mCurrentFrame(0)
{
}

SkinMeshComponent::~SkinMeshComponent() = default;

void SkinMeshComponent::start() {
    mMesh = getComponent<MeshComponent>();
    mMeshShader = getComponent<MeshShader>();

    mCurrentBones.resize(mMesh->getAnimation().getBoneCount());
}

void SkinMeshComponent::update() {
    const auto& iAnim = mMesh->getAnimation();

    const auto& motion = iAnim.getMotion(mCurrentMotionNo);
    ++mCurrentFrame;
    if (mCurrentFrame >= motion.numFrame) {
        mCurrentFrame = 0;
    }

    //シェーダーにボーンのデータを渡す
    for (size_t i = 0; i < iAnim.getBoneCount(); ++i) {
        mCurrentBones[i] = iAnim.getBone(i).offsetMat * motion.frameMat[i][mCurrentFrame];
    }
    mMeshShader->setTransferData(mCurrentBones.data(), sizeof(SkinMeshConstantBuffer), 2);
}

void SkinMeshComponent::setMotionName(const std::string& name, unsigned motionNo) {
    auto& iAnim = mMesh->getAnimation();
    assert(motionNo < iAnim.getMotionCount());
    iAnim.setMotionName(name, motionNo);
}

void SkinMeshComponent::changeMotion(unsigned motionNo) {
    assert(motionNo < mMesh->getAnimation().getMotionCount());
    mCurrentMotionNo = motionNo;
    mCurrentFrame = 0;
}

void SkinMeshComponent::changeMotion(const std::string& motionName) {
    const auto& iAnim = mMesh->getAnimation();
    for (size_t i = 0; i < iAnim.getMotionCount(); ++i) {
        if (iAnim.getMotion(i).name == motionName) {
            mCurrentMotionNo = i;
            mCurrentFrame = 0;
            return;
        }
    }

    Debug::logError("Not found motion name[" + motionName + "].");
}

const Motion& SkinMeshComponent::getCurrentMotion() const {
    return mMesh->getAnimation().getMotion(mCurrentMotionNo);
}

int SkinMeshComponent::getCurrentMotionFrame() const {
    return mCurrentFrame;
}

const std::vector<Matrix4>& SkinMeshComponent::getBoneCurrentFrameMatrix() const {
    return mCurrentBones;
}
