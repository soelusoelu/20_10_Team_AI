#pragma once

#include "../EngineMode.h"
#include "../AssetsRenderer/ICurrentSelectTextureGetter.h"
#include "../DebugManager/DebugLayer/Inspector/IInspectorTargetSetter.h"
#include "../../GameObject/IGameObjectsGetter.h"
#include "../../Math/Math.h"
#include <rapidjson/document.h>
#include <memory>

class GameObjectManager;
class MeshManager;
class SimpleCamera;
class AssetsPlacement;

class MapEditorMeshManager {
public:
    MapEditorMeshManager();
    ~MapEditorMeshManager();
    void loadProperties(const rapidjson::Value& inObj);

    void initialize(
        IInspectorTargetSetter* inspector,
        const ICurrentSelectTextureGetter* textureGetter
    );

    void update(
        EngineMode mode
    );

    void draw(
        EngineMode mode,
        const Vector3& dirLightDirection,
        const Vector3& dirLightColor
    ) const;

    //エンジン機能がマップエディタに変更されたとき
    void onChangeMapEditorMode();
    //IGameObjectsGetterを取得する
    const IGameObjectsGetter* getGameObjects() const;

private:
    MapEditorMeshManager(const MapEditorMeshManager&) = delete;
    MapEditorMeshManager& operator=(const MapEditorMeshManager&) = delete;

private:
    std::unique_ptr<GameObjectManager> mGameObjectManager;
    std::unique_ptr<MeshManager> mMeshManager;
    std::unique_ptr<SimpleCamera> mCamera;
    std::unique_ptr<AssetsPlacement> mPlace;
};
