#include "Mesh.h"
#include "../Device/AssetsManager.h"
#include "../DirectX/DirectXInclude.h"
#include "../System/World.h"
#include "../System/Shader/Shader.h"

Mesh::Mesh() :
    mMesh(nullptr),
    mShader(nullptr) {
}

Mesh::~Mesh() = default;

const Material& Mesh::getMaterial(unsigned index) const {
    return mMesh->getMaterial(index);
}

unsigned Mesh::getMeshCount() const {
    return mMesh->getMeshCount();
}

const std::vector<MeshVertices>& Mesh::getMeshesVertices() const {
    return mMeshesVertices;
}

void Mesh::loadMesh(const std::string& fileName) {
    //すでに生成済みなら終了する
    if (mMesh) {
        return;
    }

    initialize(fileName);
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
    DirectX::instance().drawIndexed(mMesh->getIndices(meshIndex).size());
}

void Mesh::initialize(const std::string& fileName) {
    createMesh(fileName);
    for (size_t i = 0; i < mMesh->getMeshCount(); i++) {
        createVertexBuffer(i);
        createIndexBuffer(i);
    }
}

void Mesh::createMesh(const std::string& fileName) {
    //アセットマネージャーからメッシュを作成する
    mMesh = World::instance().assetsManager().createMeshLoader(fileName, mMeshesVertices);
}

void Mesh::createShader(const std::string& fileName) {
    //アセットマネージャーからシェーダーを作成する
    mShader = World::instance().assetsManager().createShader(fileName);
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
    const auto& indices = mMesh->getIndices(meshIndex);
    BufferDesc bd;
    bd.size = sizeof(indices[0]) * indices.size();
    bd.usage = Usage::USAGE_DEFAULT;
    bd.type = static_cast<unsigned>(BufferType::BUFFER_TYPE_INDEX);
    SubResourceDesc sub;
    sub.data = indices.data();

    mIndexBuffers.emplace_back(std::make_unique<IndexBuffer>(bd, sub));
}
