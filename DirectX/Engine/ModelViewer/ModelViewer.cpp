#include "ModelViewer.h"
#include "AnimationViewer.h"
#include "ModelViewCamera.h"
#include "../AssetsRenderer/AssetsRenderTexture.h"
#include "../Camera/SimpleCamera.h"
#include "../../Component/Engine/Mesh/MeshComponent.h"
#include "../../Component/Engine/Mesh/MeshRenderer.h"
#include "../../GameObject/GameObject.h"
#include "../../Mesh/MeshManager.h"
#include "../../System/Texture/MeshRenderOnTexture.h"
#include "../../Transform/Transform3D.h"

ModelViewer::ModelViewer()
    : mEngineModeGetter(nullptr)
    , mAssetsTextureGetter(nullptr)
    , mMeshManager(std::make_unique<MeshManager>())
    , mModelViewCamera(std::make_unique<ModelViewCamera>())
    , mAnimationViewer(std::make_unique<AnimationViewer>())
{
}

ModelViewer::~ModelViewer() = default;

void ModelViewer::loadProperties(const rapidjson::Value& inObj) {
    mMeshManager->loadProperties(inObj);
}

void ModelViewer::initialize(
    const IEngineModeGetter* engineModeGetter,
    const ICurrentSelectTextureGetter* assetsTextureGetter,
    ICallbackSelectAssetsTexture* callbackSelectAssetsTexture
) {
    mEngineModeGetter = engineModeGetter;
    mAssetsTextureGetter = assetsTextureGetter;

    mMeshManager->initialize();
    mModelViewCamera->initialize();

    callbackSelectAssetsTexture->callbackSelectTexture([&] { onSelectAssetsTexture(); });

    //auto p = std::make_shared<GameObject>();
    ////ワールド行列の計算
    //p->transform().computeWorldTransform();
    ////床メッシュ作成
    //auto mc = Component::addComponent<MeshComponent>(*p, "MeshComponent");
    //mc->createMesh("Plane.fbx", "Assets\\Model\\Shape\\");
    //auto mr = mc->getComponent<MeshRenderer>();
    ////所有権保持のため
    //mPlane = std::make_pair(p, mr);
    ////床を影無しで追加
    //mMeshManager->add(mr, false);
}

void ModelViewer::update(EngineMode mode) {
    if (mode == EngineMode::MODEL_VIEWER) {
        if (mTarget.first) {
            mTarget.first->update();
            mTarget.first->lateUpdate();
        }

        mModelViewCamera->update();
        mAnimationViewer->update();
    }
}

void ModelViewer::setTarget(
    const GameObjectPtr& targetObj,
    const MeshRendererPtr& targetMesh
) {
    if (mTarget.second) {
        mMeshManager->erase(mTarget.second);
    }

    mTarget = std::make_pair(targetObj, targetMesh);
    mMeshManager->add(targetMesh, false);
    mModelViewCamera->onChangeModel(*targetMesh->getMeshComponent().getMesh());
    mAnimationViewer->onChangeModel(*targetObj);
}

void ModelViewer::draw(
    EngineMode mode,
    const Vector3& dirLightDirection,
    const Vector3& dirLightColor
) const {
    if (mode == EngineMode::MODEL_VIEWER) {
        const auto& camera = mModelViewCamera->getCamera();
        mMeshManager->draw(camera.getView(), camera.getProjection(), camera.getPosition(), dirLightDirection, dirLightColor);
    }
}

void ModelViewer::onSelectAssetsTexture() {
    if (mEngineModeGetter->getMode() != EngineMode::MODEL_VIEWER) {
        return;
    }
    if (!mAssetsTextureGetter->selectedTexture()) {
        return;
    }

    const auto& newTarget = std::make_shared<GameObject>();
    const auto& newMesh = Component::addComponent<MeshComponent>(*newTarget, "MeshComponent");
    newMesh->createMeshFromFilePath(mAssetsTextureGetter->getCurrentSelectTexture().getTexture().getFilePath());
    const auto& meshRenderer = newMesh->getComponent<MeshRenderer>();

    setTarget(newTarget, meshRenderer);
}

void ModelViewer::onChangeModelViewerMode() {
    mMeshManager->registerThisToMeshRenderer();
}
