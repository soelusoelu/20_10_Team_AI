#pragma once

#include <list>
#include <memory>

class MeshRenderer;
class ShadowMap;
class Camera;
class DirectionalLight;

class MeshManager {
    using MeshPtr = std::shared_ptr<MeshRenderer>;
    using MeshPtrList = std::list<MeshPtr>;

public:
    MeshManager();
    ~MeshManager();
    void update();
    void draw(const Camera& camera, const DirectionalLight& dirLight) const;
    void add(const MeshPtr& mesh);
    void addShadowMap(const std::shared_ptr<ShadowMap>& shadowMap);
    void clear();

private:
    MeshManager(const MeshManager&) = delete;
    MeshManager& operator=(const MeshManager&) = delete;

    //不要なメッシュを削除する
    void remove();
    //描画するか
    bool isDraw(const MeshRenderer& mesh, const Camera& camera) const;

private:
    MeshPtrList mMeshes;
    std::shared_ptr<ShadowMap> mShadowMap;
};
