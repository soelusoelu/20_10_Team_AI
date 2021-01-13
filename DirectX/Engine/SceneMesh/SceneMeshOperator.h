#pragma once

#include "ICurrentSelectSceneMeshGetter.h"
#include "../../Mesh/IMeshesGetter.h"
#include <memory>

class Camera;
class MeshComponent;
class SceneMeshSelector;
class SceneMeshDeleter;

class SceneMeshOperator : public ICurrentSelectSceneMeshGetter {
public:
    SceneMeshOperator();
    ~SceneMeshOperator();
    virtual const MeshComponent& getCurrentSelectMesh() const override;
    virtual bool selectedMesh() const override;
    void initialize(const std::shared_ptr<Camera>& camera, const IMeshesGetter* getter);
    void update();

private:
    SceneMeshOperator(const SceneMeshOperator&) = delete;
    SceneMeshOperator& operator=(const SceneMeshOperator&) = delete;

private:
    std::unique_ptr<SceneMeshSelector> mSelector;
    std::unique_ptr<SceneMeshDeleter> mDeleter;
    std::shared_ptr<MeshComponent> mCurrentSelectMesh;
};
