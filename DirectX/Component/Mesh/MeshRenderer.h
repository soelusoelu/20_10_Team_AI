﻿#pragma once

#include "IDrawBefore.h"
#include "../Component.h"
#include "../../Mesh/IMesh.h"
#include "../../Mesh/IMeshDrawer.h"
#include <memory>

class MeshComponent;
class MeshShader;
class Camera;
class DirectionalLight;
class MeshManager;

//メッシュ描画専用クラス
class MeshRenderer : public Component, std::enable_shared_from_this<MeshRenderer> {
public:
    MeshRenderer(GameObject& gameObject);
    ~MeshRenderer();
    virtual void start() override;

    //描画
    void draw(const Camera& camera, const DirectionalLight& dirLight) const;
    //描画前描画をしたいクラスを登録する
    void setDrawBefore(const IDrawBefore* drawer);

    //自身を管理するマネージャーを登録する
    static void setMeshManager(MeshManager* manager);

private:
    MeshRenderer(const MeshRenderer&) = delete;
    MeshRenderer& operator=(const MeshRenderer&) = delete;

    //メインメッシュ描画
    void drawMesh(const Camera& camera, const DirectionalLight& dirLight) const;
    //スキンメッシュコンポーネントを条件に合えば作る
    void createSkinMeshComponent();
    //マネージャーに登録する
    void addToManager();

private:
    const IMesh* mMesh;
    const IMeshDrawer* mDrawer;
    const IDrawBefore* mBeforeDrawer;
    std::shared_ptr<MeshComponent> mMeshComponent;
    std::shared_ptr<MeshShader> mMeshShader;

    static inline MeshManager* mMeshManager = nullptr;
};
