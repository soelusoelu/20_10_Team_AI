#pragma once

#include <memory>
#include <string>
#include <unordered_map>

class Mesh;
class TextureFromFile;

class AssetsManager {
private:
    //シングルトンだからprivate
    AssetsManager();
public:
    ~AssetsManager();
    //アセット管理クラスのインスタンスを返す
    static AssetsManager& instance();
    //終了処理
    void finalize();
    //ファイルパスからテクスチャを取得する
    std::shared_ptr<TextureFromFile> createTexture(const std::string& fileName, const std::string& directoryPath = "Assets\\Texture\\");
    //ファイルパスからメッシュを取得する
    std::shared_ptr<Mesh> createMesh(const std::string& fileName, const std::string& directoryPath = "Assets\\Model\\");

private:
    AssetsManager(const AssetsManager&) = delete;
    AssetsManager& operator=(const AssetsManager&) = delete;

private:
    static inline AssetsManager* mInstance = nullptr;

    std::unordered_map<std::string, std::shared_ptr<TextureFromFile>> mTextures;
    std::unordered_map<std::string, std::shared_ptr<Mesh>> mMeshes;
};
