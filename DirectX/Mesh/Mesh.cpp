#include "Mesh.h"
#include "FBX.h"
#include "OBJ.h"
#include "../DebugLayer/Debug.h"
#include "../DirectX/DirectXInclude.h"
#include "../System/Shader/Shader.h"
#include "../Utility/FileUtil.h"
#include <cassert>

Mesh::Mesh() :
    mMesh(nullptr),
    mShader(nullptr) {
}

Mesh::~Mesh() = default;

const Material& Mesh::getMaterial(unsigned index) const {
    return mMaterials[index];
}

unsigned Mesh::getMeshCount() const {
    return mMeshesVertices.size();
}

const std::vector<MeshVertices>& Mesh::getMeshesVertices() const {
    return mMeshesVertices;
}

void Mesh::loadMesh(const std::string& filePath) {
    //すでに生成済みなら終了する
    if (mMesh) {
        return;
    }

    initialize(filePath);
}

void Mesh::loadShader(const std::string& shaderName) {
    createShader(shaderName);
}

void Mesh::setShaderData(const void* data, unsigned size, unsigned index) const {
    //シェーダーにデータを転送する
    mShader->transferData(data, size, index);
}

void Mesh::draw(unsigned meshIndex) const {
    //使用するシェーダーの登録
    mShader->setShaderInfo();
    //バーテックスバッファーをセット
    mVertexBuffers[meshIndex]->setVertexBuffer();
    //インデックスバッファーをセット
    mIndexBuffers[meshIndex]->setIndexBuffer();

    //プリミティブをレンダリング
    MyDirectX::DirectX::instance().drawIndexed(mMeshesIndices[meshIndex].size());
}

void Mesh::initialize(const std::string& filePath) {
    //ファイルパスからメッシュを作成
    createMesh(filePath);

    //それぞれは同じサイズのはず
    assert(mMeshesVertices.size() == mMeshesIndices.size() == mMaterials.size());

    //メッシュの数だけバッファを作る
    for (size_t i = 0; i < mMeshesVertices.size(); i++) {
        createVertexBuffer(i);
        createIndexBuffer(i);
    }
}

void Mesh::createMesh(const std::string& filePath) {
    //拡張子によって処理を分ける
    auto ext = FileUtil::getFileExtension(filePath);
    if (ext == ".obj") {
        mMesh = std::make_unique<OBJ>();
    } else if (ext == ".fbx") {
        mMesh = std::make_unique<FBX>();
    } else {
        Debug::windowMessage(filePath + ": 対応していない拡張子です");
    }

    //メッシュを解析する
    mMesh->perse(filePath, mMeshesVertices, mMeshesIndices, mMaterials);
}

void Mesh::createShader(const std::string& fileName) {
    mShader = std::make_unique<Shader>(fileName);
}

void Mesh::createVertexBuffer(unsigned meshIndex) {
    const auto& vertices = mMeshesVertices[meshIndex];
    BufferDesc bd;
    bd.oneSize = sizeof(vertices[0]);
    bd.size = bd.oneSize * vertices.size();
    bd.usage = Usage::USAGE_DEFAULT;
    bd.type = static_cast<unsigned>(BufferType::BUFFER_TYPE_VERTEX);
    SubResourceDesc sub;
    sub.data = vertices.data();

    mVertexBuffers.emplace_back(std::make_unique<VertexBuffer>(bd, sub));
}

void Mesh::createIndexBuffer(unsigned meshIndex) {
    const auto& indices = mMeshesIndices[meshIndex];
    BufferDesc bd;
    bd.size = sizeof(indices[0]) * indices.size();
    bd.usage = Usage::USAGE_DEFAULT;
    bd.type = static_cast<unsigned>(BufferType::BUFFER_TYPE_INDEX);
    SubResourceDesc sub;
    sub.data = indices.data();

    mIndexBuffers.emplace_back(std::make_unique<IndexBuffer>(bd, sub));
}
