﻿#pragma once

#include "../AssetsDirectoryPath.h"
#include "../../Math/Math.h"
#include "../../Mesh/IMesh.h"
#include <memory>
#include <string>

class RenderTexture;
class Sprite;
class Mesh;
class Shader;

//メッシュをテクスチャ上に描画する
class MeshRenderOnTexture {
public:
    MeshRenderOnTexture(const std::string& filePath, int width, int height);
    MeshRenderOnTexture(const std::string& fileName, const std::string& directoryPath, int width, int height);
    ~MeshRenderOnTexture();
    //メッシュをテクスチャ上に描画する
    void drawMeshOnTexture(const Matrix4& viewProj) const;
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

private:
    MeshRenderOnTexture(const MeshRenderOnTexture&) = delete;
    MeshRenderOnTexture& operator=(const MeshRenderOnTexture&) = delete;

private:
    std::unique_ptr<RenderTexture> mRenderTexture;
    std::unique_ptr<Sprite> mSprite;
    std::shared_ptr<Mesh> mMesh;
    std::shared_ptr<Shader> mMeshShader;
    std::string mFilePath;
    int mWidth;
    int mHeight;
};
