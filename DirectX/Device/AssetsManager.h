#pragma once

#include <memory>
#include <string>
#include <unordered_map>

class Mesh;
class TextureFromFile;

class AssetsManager {
public:
    AssetsManager();
    ~AssetsManager();
    std::shared_ptr<TextureFromFile> createTexture(const std::string& fileName, const std::string& directoryPath = "Assets\\Texture\\");
    std::shared_ptr<Mesh> createMesh(const std::string& fileName, const std::string& directoryPath = "Assets\\Model\\");

private:
    AssetsManager(const AssetsManager&) = delete;
    AssetsManager& operator=(const AssetsManager&) = delete;

private:
    std::unordered_map<std::string, std::shared_ptr<TextureFromFile>> mTextures;
    std::unordered_map<std::string, std::shared_ptr<Mesh>> mMeshes;

    static inline bool mInstantiated = false;
};
