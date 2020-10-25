#include "AssetsManager.h"
#include "../Mesh/Mesh.h"
#include "../System/GlobalFunction.h"
#include "../System/Texture/TextureFromFile.h"

AssetsManager::AssetsManager() = default;

AssetsManager::~AssetsManager() = default;

AssetsManager& AssetsManager::instance() {
    if (!mInstance) {
        mInstance = new AssetsManager();
    }
    return *mInstance;
}

void AssetsManager::finalize() {
    safeDelete(mInstance);
}

std::shared_ptr<TextureFromFile> AssetsManager::createTexture(const std::string & fileName, const std::string& directoryPath) {
    std::shared_ptr<TextureFromFile> texture = nullptr;
    auto filePath = directoryPath + fileName;
    auto itr = mTextures.find(filePath);
    if (itr != mTextures.end()) { //既に読み込まれている
        texture = itr->second;
    } else { //初読み込み
        texture = std::make_shared<TextureFromFile>(filePath);
        mTextures.emplace(filePath, texture);
    }
    return texture;
}

std::shared_ptr<Mesh> AssetsManager::createMesh(const std::string& fileName, const std::string& directoryPath) {
    std::shared_ptr<Mesh> mesh = nullptr;
    auto filePath = directoryPath + fileName;
    auto itr = mMeshes.find(filePath);
    if (itr != mMeshes.end()) { //既に読み込まれている
        mesh = itr->second;
    } else { //初読み込み
        mesh = std::make_shared<Mesh>();
        mesh->loadMesh(filePath);
        mMeshes.emplace(filePath, mesh);
    }
    return mesh;
}
