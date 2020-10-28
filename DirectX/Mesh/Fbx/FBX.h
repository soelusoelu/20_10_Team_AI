#pragma once

#include "../IMeshLoader.h"
#include <fbxsdk.h>
#include <memory>
#include <string>

class FbxMeshParser;
class FbxMaterialParser;
class FbxBoneParser;

class FBX : public IMeshLoader {
public:
    FBX();
    ~FBX();
    virtual void parse(
        const std::string& filePath,
        std::vector<MeshVertices>& meshesVertices,
        std::vector<Indices>& meshesIndices,
        std::vector<Material>& materials
    ) override;

private:
    //すべてのメッシュを作成する
    void createMeshes(
        std::vector<MeshVertices>& meshesVertices,
        std::vector<Indices>& meshesIndices,
        std::vector<Material>& materials,
        const FbxScene* fbxScene,
        const std::string& filePath
    );

    //メッシュ作成
    void createMesh(
        MeshVertices& meshVertices,
        Indices& indices,
        Material& material,
        FbxMesh* fbxMesh,
        const std::string& directoryPath
    );

private:
    std::unique_ptr<FbxMeshParser> mMeshParser;
    std::unique_ptr<FbxMaterialParser> mMaterialParser;
    std::unique_ptr<FbxBoneParser> mBoneParser;
};
