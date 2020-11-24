#include "SkinMeshComponent.h"
#include "MeshComponent.h"
#include "MeshShader.h"
#include "../../DebugLayer/Debug.h"
#include "../../Mesh/IMesh.h"
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
    if (!mMesh) {
        mMesh = getComponent<MeshComponent>();
    }
    if (!mMeshShader) {
        mMeshShader = getComponent<MeshShader>();
    }

    mCurrentBones.resize(mMesh->getMesh().getBoneCount());
}

void SkinMeshComponent::update() {
    const auto& iMesh = mMesh->getMesh();

    const auto& motion = iMesh.getMotion(mCurrentMotionNo);
    ++mCurrentFrame;
    if (mCurrentFrame >= motion.numFrame) {
        mCurrentFrame = 0;
    }

    //シェーダーにボーンのデータを渡す
    for (size_t i = 0; i < iMesh.getBoneCount(); ++i) {
        mCurrentBones[i] = iMesh.getBone(i).offsetMat * motion.frameMat[i][mCurrentFrame];
    }
    mMeshShader->setTransferData(mCurrentBones.data(), sizeof(SkinMeshConstantBuffer), 2);
}

void SkinMeshComponent::setMotionName(const std::string& name, unsigned motionNo) {
    auto& iMesh = mMesh->getMesh();
    assert(motionNo < iMesh.getMotionCount());
    iMesh.setMotionName(name, motionNo);
}

void SkinMeshComponent::changeMotion(unsigned motionNo) {
    assert(motionNo < mMesh->getMesh().getMotionCount());
    mCurrentMotionNo = motionNo;
    mCurrentFrame = 0;
}

void SkinMeshComponent::changeMotion(const std::string& motionName) {
    const auto& iMesh = mMesh->getMesh();
    for (size_t i = 0; i < iMesh.getMotionCount(); ++i) {
        if (iMesh.getMotion(i).name == motionName) {
            mCurrentMotionNo = i;
            mCurrentFrame = 0;
            return;
        }
    }

    Debug::logError("Not found motion name[" + motionName + "].");
}

const Motion& SkinMeshComponent::getCurrentMotion() const {
    return mMesh->getMesh().getMotion(mCurrentMotionNo);
}

int SkinMeshComponent::getCurrentMotionFrame() const {
    return mCurrentFrame;
}

const std::vector<Matrix4>& SkinMeshComponent::getBoneCurrentFrameMatrix() const {
    return mCurrentBones;
}

void SkinMeshComponent::setMeshAndShader(
    const std::shared_ptr<MeshComponent>& mesh,
    const std::shared_ptr<MeshShader>& shader
) {
    mMesh = mesh;
    mMeshShader = shader;
}
