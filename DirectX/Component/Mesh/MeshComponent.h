﻿#pragma once

#include "../Component.h"
#include "../../Math/Math.h"
#include "../../Mesh/IMesh.h"
#include "../../Mesh/Material.h"
#include <memory>
#include <string>
#include <vector>

class Mesh;
class MeshManager;
class Shader;
class Camera;
class DirectionalLight;

class MeshComponent : public Component, public std::enable_shared_from_this<MeshComponent> {
    enum class State {
        ACTIVE,
        NON_ACTIVE,
        DEAD
    };

public:
    MeshComponent(GameObject& gameObject);
    virtual ~MeshComponent();
    virtual void awake() override;
    virtual void start() override;
    virtual void onEnable(bool value) override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;

    //描画
    virtual void draw(const Camera& camera, const DirectionalLight& dirLight) const;

    //状態
    void destroy();
    void setActive(bool value);
    bool getActive() const;
    bool isDead() const;
    //メッシュインターフェースを取得する
    const IMesh& getMesh() const;
    //アルファ値を設定する
    void setAlpha(float alpha);
    //アルファ値を取得する
    float getAlpha() const;

    //自身を管理するマネージャーを登録する
    static void setMeshManager(MeshManager* manager);

private:
    void addToManager();

protected:
    std::shared_ptr<Mesh> mMesh;
    std::unique_ptr<Shader> mShader;
    std::string mFileName;
    State mState;
    float mAlpha;

    static inline MeshManager* mMeshManager = nullptr;
};
