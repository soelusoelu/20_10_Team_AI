#pragma once

#include <memory>

class MeshComponent;

class SceneMeshDeleter {
public:
    SceneMeshDeleter();
    ~SceneMeshDeleter();
    void deleteMesh(std::shared_ptr<MeshComponent>& target);

private:
    SceneMeshDeleter(const SceneMeshDeleter&) = delete;
    SceneMeshDeleter& operator=(const SceneMeshDeleter&) = delete;
};
