#pragma once

#include "IMesh.h"
#include "IMeshLoader.h"
#include "Material.h"
#include "../Math/Math.h"
#include <memory>
#include <string>
#include <vector>

class Shader;
class VertexBuffer;
class IndexBuffer;

class Mesh : public IMesh {
public:
    Mesh();
    ~Mesh();

    //マテリアルの取得
    virtual const Material& getMaterial(unsigned index) const override;
    //メッシュの数を取得する
    virtual unsigned getMeshCount() const override;
    //すべての頂点情報を取得
    virtual const std::vector<MeshVertices>& getMeshesVertices() const override;

    //ファイル名からメッシュを生成する
    void loadMesh(const std::string& fileName);
    //シェーダー名からシェーダーを作成する
    void loadShader(const std::string& shaderName);
    //コンスタントバッファを設定する
    void setShaderData(const void* data, unsigned size, unsigned index = 0) const;
    //メッシュを描画する
    void draw(unsigned meshIndex) const;

private:
    //初期化処理
    void initialize(const std::string& fileName);
    //メッシュを生成する
    void createMesh(const std::string& fileName);
    //シェーダーを生成する
    void createShader(const std::string& fileName);
    //バーテックスバッファを生成する
    void createVertexBuffer(unsigned meshIndex);
    //インデックスバッファを生成する
    void createIndexBuffer(unsigned meshIndex);

private:
    std::unique_ptr<IMeshLoader> mMesh;
    std::vector<MeshVertices> mMeshesVertices;
    std::unique_ptr<Shader> mShader;
    std::vector<std::unique_ptr<VertexBuffer>> mVertexBuffers;
    std::vector<std::unique_ptr<IndexBuffer>> mIndexBuffers;
};
