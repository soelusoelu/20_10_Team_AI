#pragma once

#include "../../Component.h"
#include "../../../Math/Math.h"
#include "../../../Mesh/IMesh.h"
#include <memory>
#include <string>

class MeshRenderOnTexture;
class MeshRenderOnTextureManager;
class Sprite;

class MeshRenderOnTextureComponent : public Component, public std::enable_shared_from_this<MeshRenderOnTextureComponent> {
public:
    MeshRenderOnTextureComponent(GameObject& gameObject);
    ~MeshRenderOnTextureComponent();
    virtual void start() override;
    virtual void finalize() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;

    //メッシュをテクスチャ上に描画する
    void drawMeshOnTexture(const Matrix4& viewProj) const;
    //メッシュ描画済みテクスチャを描画する
    void draw(const Matrix4& proj) const;
    //描画するメッシュを変更する
    void changeMesh(const std::string& filePath);
    //テクスチャの位置を設定する
    void setPositionForTexture(const Vector2& pos);
    //テクスチャのサイズを設定する
    void setSizeForTexture(const Vector2& size);
    //メッシュを取得する
    const IMesh& getMesh() const;
    //スプライトを取得する
    const Sprite& getSprite() const;
    //ファイルパスを取得する
    const std::string& getFilePath() const;

    //自身を管理するマネージャーを登録する
    static void setMeshRenderOnTextureManager(MeshRenderOnTextureManager* manager);

private:
    MeshRenderOnTextureComponent(const MeshRenderOnTextureComponent&) = delete;
    MeshRenderOnTextureComponent& operator=(const MeshRenderOnTextureComponent&) = delete;

private:
    std::unique_ptr<MeshRenderOnTexture> mMeshRenderOnTexture;

    static inline MeshRenderOnTextureManager* mManager = nullptr;

    static constexpr int DEFAULT_SPRITE_WIDTH = 128;
    static constexpr int DEFAULT_SPRITE_HEIGHT = 128;
};
