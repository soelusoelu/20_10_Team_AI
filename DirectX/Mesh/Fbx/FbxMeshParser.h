#pragma once

#include "../IMeshLoader.h"
#include <fbxsdk.h>
#include <unordered_map>
#include <vector>

//FBXメッシュ解析クラス
class FbxMeshParser {
    using Normals = std::unordered_map<unsigned short, Vector3>;
    using UVs = std::unordered_map<unsigned short, Vector2>;

public:
    FbxMeshParser();
    ~FbxMeshParser();

    //メッシュを解析する
    void parse(
        MeshVertices& meshVertices,
        Indices& indices,
        FbxMesh* fbxMesh
    ) const;

private:
    //現状使用してない(サンプル確認用)
    void loadNormal(FbxMesh* mesh);
    void loadUV(FbxMesh* mesh);

    //インデックスバッファを取得する
    void getIndices(
        Indices& indices,
        const FbxMesh* fbxMesh
    ) const;

    //法線とUVを取得する
    void getNormalsAndUVs(
        Normals& normals,
        UVs& uvs,
        const Indices& indices,
        const FbxMesh* fbxMesh
    ) const;

    //UV配列を作成する
    void getUVArray(
        FbxArray<FbxVector2>& uvArray,
        const FbxMesh* fbxMesh
    ) const;
};
