#pragma once

#include "../../Mesh/IMeshesGetter.h"
#include <memory>

class Camera;
class MeshComponent;

class SceneMeshSelector {
public:
    SceneMeshSelector();
    ~SceneMeshSelector();
    void initialize(const std::shared_ptr<Camera>& camera, const IMeshesGetter* getter);
    //メッシュを選択する
    bool selectMesh(std::shared_ptr<MeshComponent>& out);

private:
    SceneMeshSelector(const SceneMeshSelector&) = delete;
    SceneMeshSelector& operator=(const SceneMeshSelector&) = delete;

    //メッシュを選択する条件が整っているか
    bool selectConditions() const;

private:
    std::shared_ptr<Camera> mCamera;
    const IMeshesGetter* mMeshesGetter;
};
