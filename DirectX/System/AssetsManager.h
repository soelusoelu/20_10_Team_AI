#pragma once

#include "AssetsDirectoryPath.h"
#include <memory>
#include <string>
#include <unordered_map>

class Mesh;
class TextureFromFile;
class Shader;

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

    //テクスチャを読み込む
    void loadTexture(const std::string& fileName, const std::string& directoryPath = AssetsDirectoryPath::TEXTURE_PATH);
    //ファイルパスからテクスチャを取得する
    std::shared_ptr<TextureFromFile> createTexture(const std::string& fileName, const std::string& directoryPath = AssetsDirectoryPath::TEXTURE_PATH);

    //メッシュを読み込む
    void loadMesh(const std::string& fileName, const std::string& directoryPath = AssetsDirectoryPath::MODEL_PATH);
    //メッシュを読み込む
    void loadMeshFromFilePath(const std::string& filePath);
    //ファイルパスからメッシュを取得する
    std::shared_ptr<Mesh> createMesh(const std::string& fileName, const std::string& directoryPath = AssetsDirectoryPath::MODEL_PATH);
    //ファイルパスからメッシュを取得する
    std::shared_ptr<Mesh> createMeshFromFilePath(const std::string& filePath);

    //ファイルパスからシェーダーを取得する
    std::shared_ptr<Shader> createShader(const std::string& fileName, const std::string& directoryPath = AssetsDirectoryPath::SHADER_PATH);

private:
    AssetsManager(const AssetsManager&) = delete;
    AssetsManager& operator=(const AssetsManager&) = delete;

    //読み込み済みのテクスチャか
    bool loadedTexture(const std::string& filePath) const;
    //読み込み済みのメッシュか
    bool loadedMesh(const std::string& filePath) const;

private:
    static inline AssetsManager* mInstance = nullptr;

    std::unordered_map<std::string, std::shared_ptr<TextureFromFile>> mTextures;
    std::unordered_map<std::string, std::shared_ptr<Mesh>> mMeshes;
    std::unordered_map<std::string, std::shared_ptr<Shader>> mShaders;
};
