#include "SkinMeshComponent.h"
#include "MeshComponent.h"
#include "MeshShader.h"
#include "../../Input/Input.h"
#include "../../Mesh/Mesh.h"
#include "../../System/Shader/ConstantBuffers.h"

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
}

void SkinMeshComponent::update() {
    const auto& iMesh = mMesh->getMesh();

    if (Input::keyboard().getKeyDown(KeyCode::Space)) {
        mCurrentMotionNo = ++mCurrentMotionNo % iMesh.getMotionCount();
        mCurrentFrame = 0;
    }
    const auto& motion = iMesh.getMotion(mCurrentMotionNo);
    ++mCurrentFrame;
    if (mCurrentFrame >= motion.numFrame) {
        mCurrentFrame = 0;
    }

    //シェーダーにボーンのデータを渡す
    SkinMeshConstantBuffer skinMeshcb;
    for (size_t i = 0; i < iMesh.getBoneCount(); i++) {
        skinMeshcb.bones[i] = iMesh.getBone(i).offsetMat * motion.frameMat[i][mCurrentFrame];
    }
    mMeshShader->setTransferData(&skinMeshcb, sizeof(skinMeshcb), 2);
}

void SkinMeshComponent::setMeshAndShader(
    const std::shared_ptr<MeshComponent>& mesh,
    const std::shared_ptr<MeshShader>& shader
) {
    mMesh = mesh;
    mMeshShader = shader;
}
