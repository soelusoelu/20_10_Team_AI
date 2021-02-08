#include "EngineFunctionManager.h"
#include "EngineFuctionChanger.h"
#include "AssetsRenderer/AssetsRenderTextureManager.h"
#include "DebugManager/DebugManager.h"
#include "DebugManager/DebugLayer/DebugLayer.h"
#include "MapEditor/MapEditorMeshManager.h"
#include "ModelViewer/ModelViewer.h"
#include "Pause/Pause.h"
#include "SceneMesh/SceneMeshOperator.h"
#include "../Component/Engine/Camera/Camera.h"
#include "../Component/Engine/Light/DirectionalLight.h"
#include "../Device/Renderer.h"

EngineFunctionManager::EngineFunctionManager()
    : mDebugManager(std::make_unique<DebugManager>())
    , mPause(std::make_unique<Pause>())
    , mFunctionChanger(std::make_unique<EngineFuctionChanger>())
    , mMapEditor(std::make_unique<MapEditorMeshManager>())
    , mAssetsRenderTextureManager(std::make_unique<AssetsRenderTextureManager>())
    , mSceneMeshOperator(std::make_unique<SceneMeshOperator>())
    , mModelViewer(std::make_unique<ModelViewer>())
{
}

EngineFunctionManager::~EngineFunctionManager() = default;

void EngineFunctionManager::loadProperties(const rapidjson::Value& inObj) {
    mDebugManager->loadProperties(inObj);
    mPause->loadProperties(inObj);
    mFunctionChanger->loadProperties(inObj);
    mMapEditor->loadProperties(inObj);
    mAssetsRenderTextureManager->loadProperties(inObj);
    mModelViewer->loadProperties(inObj);
}

void EngineFunctionManager::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) {
    mDebugManager->saveProperties(alloc, inObj);
    mPause->saveProperties(alloc, inObj);
    mFunctionChanger->saveProperties(alloc, inObj);
    mAssetsRenderTextureManager->saveProperties(alloc, inObj);
}

void EngineFunctionManager::initialize(
    const std::shared_ptr<Camera>& camera,
    ICallbackChangeEngineMode* callback,
    const IEngineModeGetter* engineModeGetter,
    const IGameObjectsGetter* gameObjctsGetter,
    const IMeshesGetter* meshesGetter,
    const IFpsGetter* fpsGetter
) {
#ifdef _DEBUG
    mDebugManager->initialize(gameObjctsGetter, fpsGetter, mPause.get());
    mPause->initialize();
    mFunctionChanger->initialize(callback);
    mMapEditor->initialize(mDebugManager->getDebugLayer().inspector(), mAssetsRenderTextureManager.get());
    mAssetsRenderTextureManager->initialize();
    mSceneMeshOperator->initialize(camera, meshesGetter);
    mModelViewer->initialize(engineModeGetter, mAssetsRenderTextureManager.get(), mAssetsRenderTextureManager->getCallbackSelectAssetsTexture());
#endif // _DEBUG
}

void EngineFunctionManager::preUpdateProcess() {
#ifdef _DEBUG
    mDebugManager->preUpdateProcess();
#endif // _DEBUG
}

void EngineFunctionManager::update(EngineMode mode) {
#ifdef _DEBUG
    mAssetsRenderTextureManager->update(mode);
    mMapEditor->update(mode);
    mModelViewer->update(mode);
    mDebugManager->update();
    mPause->update();
    mFunctionChanger->update();
    mSceneMeshOperator->update();
#endif // _DEBUG
}

void EngineFunctionManager::draw(EngineMode mode, const Renderer& renderer, Matrix4& proj) const {
#ifdef _DEBUG
    //レンダリング領域をデバッグに変更
    renderer.renderToDebug(proj);

    mAssetsRenderTextureManager->drawTextures(mode, proj);
    mPause->drawButton(proj);
    mFunctionChanger->draw(proj);
    mDebugManager->draw(mode, renderer, proj);
#endif // _DEBUG
}

void EngineFunctionManager::draw3D(
    EngineMode mode,
    const Renderer& renderer,
    const Camera& camera,
    const DirectionalLight& dirLight
) const {
#ifdef _DEBUG
    mAssetsRenderTextureManager->drawMeshes(mode);
    mMapEditor->draw(mode, dirLight.getDirection(), dirLight.getLightColor());
    mModelViewer->draw(mode, dirLight.getDirection(), dirLight.getLightColor());
    mDebugManager->draw3D(mode, renderer, camera.getViewProjection());
#endif // _DEBUG
}

void EngineFunctionManager::onChangeMapEditorMode() {
    mMapEditor->onChangeMapEditorMode();
}

void EngineFunctionManager::onChangeModelViewerMode() {
    mModelViewer->onChangeModelViewerMode();
}

DebugManager& EngineFunctionManager::debug() const {
    return *mDebugManager;
}

IPause& EngineFunctionManager::pause() const {
    return *mPause;
}

AssetsRenderTextureManager& EngineFunctionManager::getAssetsRenderTextureManager() const {
    return *mAssetsRenderTextureManager;
}

MapEditorMeshManager& EngineFunctionManager::getMapEditorMeshManager() const {
    return *mMapEditor;
}
