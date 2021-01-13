#pragma once

class MeshComponent;

class ICurrentSelectSceneMeshGetter {
public:
    virtual ~ICurrentSelectSceneMeshGetter() = default;
    virtual const MeshComponent& getCurrentSelectMesh() const = 0;
    virtual bool selectedMesh() const = 0;
};
