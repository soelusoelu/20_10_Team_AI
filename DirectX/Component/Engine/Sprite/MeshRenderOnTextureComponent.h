#pragma once

#include "../../../System/AssetsDirectoryPath.h"
#include "../../Component.h"
#include "../../../Math/Math.h"
#include "../../../Mesh/IMesh.h"
#include <memory>
#include <string>

class MeshRenderOnTexture;
class MeshRenderOnTextureManager;
class Sprite;
struct Sphere;

class MeshRenderOnTextureComponent : public Component, public std::enable_shared_from_this<MeshRenderOnTextureComponent> {
public:
    MeshRenderOnTextureComponent(GameObject& gameObject);
    ~MeshRenderOnTextureComponent();
    virtual void start() override;
    virtual void update() override;
    virtual void finalize() override;
    virtual void onEnable(bool value) override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;

    //メッシュをテクスチャ上に描画する
    void drawMeshOnTexture() const;
    //メッシュ描画済みテクスチャを描画する
    void draw(const Matrix4& proj) const;
    //描画するメッシュを変更する
    void changeMeshFromFilePath(const std::string& filePath);
    void changeMesh(const std::string& fileName, const std::string& directoryPath = AssetsDirectoryPath::MODEL_PATH);
    //テクスチャの位置を設定する
    void setPositionForTexture(const Vector2& pos);
    //テクスチャのサイズを設定する
    void setSizeForTexture(const Vector2& size);
    //スプライトを取得する
    Sprite& getSprite() const;
    //メッシュを取得する
    const IMesh& getMesh() const;
    //ファイルパスを取得する
    const std::string& getFilePath() const;
    //テクスチャの横幅を取得する
    int getWidth() const;
    //テクスチャの縦幅を取得する
    int getHeight() const;

    //自身を管理するマネージャーを登録する
    static void setMeshRenderOnTextureManager(MeshRenderOnTextureManager* manager);

private:
    MeshRenderOnTextureComponent(const MeshRenderOnTextureComponent&) = delete;
    MeshRenderOnTextureComponent& operator=(const MeshRenderOnTextureComponent&) = delete;

    void calcView();

private:
    std::unique_ptr<MeshRenderOnTexture> mMeshRenderOnTexture;
    Sphere mSphereCoverMesh;
    Matrix4 mView;
    Matrix4 mProjection;

    static inline MeshRenderOnTextureManager* mManager = nullptr;

    static constexpr int DEFAULT_SPRITE_WIDTH = 128;
    static constexpr int DEFAULT_SPRITE_HEIGHT = 128;
};
