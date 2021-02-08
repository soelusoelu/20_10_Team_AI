#include "MapEditorMeshManager.h"
#include "AssetsPlacement.h"
#include "../Camera/SimpleCamera.h"
#include "../../GameObject/GameObjectManager.h"
#include "../../Mesh/MeshManager.h"

MapEditorMeshManager::MapEditorMeshManager()
    : mGameObjectManager(std::make_unique<GameObjectManager>(false))
    , mMeshManager(std::make_unique<MeshManager>())
    , mCamera(std::make_unique<SimpleCamera>())
    , mPlace(std::make_unique<AssetsPlacement>())
{
    mCamera->setPosition(Vector3::up * 15.f + Vector3::back * 15.f);
    mCamera->lookAt(Vector3::forward * 5.f);
}

MapEditorMeshManager::~MapEditorMeshManager() = default;

void MapEditorMeshManager::loadProperties(const rapidjson::Value& inObj) {
    mMeshManager->loadProperties(inObj);
}

void MapEditorMeshManager::initialize(
    IInspectorTargetSetter* inspector,
    const ICurrentSelectTextureGetter* textureGetter
) {
    mMeshManager->initialize();
    mPlace->initialize(mGameObjectManager.get(), mMeshManager.get(), inspector, textureGetter);
}

void MapEditorMeshManager::update(
    EngineMode mode
) {
    if (mode == EngineMode::MAP_EDITOR) {
        mPlace->placeAsset(*mCamera, mMeshManager.get());
        mGameObjectManager->update();
    }
}

void MapEditorMeshManager::draw(
    EngineMode mode,
    const Vector3& dirLightDirection,
    const Vector3& dirLightColor
) const {
    if (mode == EngineMode::MAP_EDITOR) {
        mMeshManager->draw(
            mCamera->getView(),
            mCamera->getProjection(),
            mCamera->getPosition(),
            dirLightDirection,
            dirLightColor
        );
    }
}

void MapEditorMeshManager::onChangeMapEditorMode() {
    mMeshManager->registerThisToMeshRenderer();
}

const IGameObjectsGetter* MapEditorMeshManager::getGameObjects() const {
    return mGameObjectManager.get();
}
