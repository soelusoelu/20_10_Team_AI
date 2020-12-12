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
    void createShadowMap();
    void update();
    void draw(const Camera& camera, const DirectionalLight& dirLight) const;
    void add(const MeshPtr& mesh, bool handleShadow);
    void clear();

private:
    MeshManager(const MeshManager&) = delete;
    MeshManager& operator=(const MeshManager&) = delete;

    //不要なメッシュを削除する
    void remove();
    //描画するか
    bool isDraw(const MeshRenderer& mesh, const Camera& camera) const;
    //メッシュの描画をする
    void drawMeshes(const Camera& camera, const DirectionalLight& dirLight) const;
    //テクスチャに影を描画する
    void drawShadow(const Camera& camera, const DirectionalLight& dirLight) const;

private:
    //影の影響を受けるメッシュリスト
    MeshPtrList mShadowMeshes;
    //影の影響を受けないメッシュリスト
    MeshPtrList mMeshes;
    std::shared_ptr<ShadowMap> mShadowMap;
};
