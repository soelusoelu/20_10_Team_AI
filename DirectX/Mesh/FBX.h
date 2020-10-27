#pragma once

#include "IMeshLoader.h"
#include "../Math/Math.h"
#include <fbxsdk.h>
#include <memory>
#include <string>
#include <vector>

//ボーン構造体
struct Bone {
    std::string name;
    Vector3 startPos;
    Vector3 endPos;
    std::vector<Bone> children;
};

//アニメーション
struct Motion {
    unsigned numFrame; //フレーム数
    std::vector<Matrix4> key; //キーフレーム
};

class FBX : public IMeshLoader {
public:
    FBX();
    ~FBX();
    virtual void perse(
        const std::string& filePath,
        std::vector<MeshVertices>& meshesVertices,
        std::vector<Indices>& meshesIndices,
        std::vector<Material>& materials
    ) override;

private:
    //メッシュ作成
    void createMesh(
        MeshVertices& meshVertices,
        Indices& indices,
        Material& material,
        FbxMesh* fbxMesh,
        const std::string& directoryPath
    );

    //現状使用してない(サンプル確認用)
    void loadNormal(FbxMesh* mesh);
    void loadUV(FbxMesh* mesh);
    void computeIndices(Indices& indices, FbxMesh* fbxMesh);

    //頂点情報 & 添字読み込み
    void loadFace(
        MeshVertices& meshVertices,
        Indices& indices,
        FbxMesh* fbxMesh
    );

    //マテリアル読み込み
    void loadMaterial(
        Material& material,
        FbxMesh* fbxMesh,
        const std::string& directoryPath
    );
    void loadMaterialAttribute(Material& material, FbxSurfaceMaterial* fbxSurfaceMaterial);
    void loadLambert(Material& material, const FbxSurfaceLambert* fbxSurfaceLambert);
    void loadPhong(Material& material, const FbxSurfacePhong* fbxSurfacePhong);

    //テクスチャ読み込み
    void loadTextures(
        Material& material,
        FbxSurfaceMaterial* fbxSurfaceMaterial,
        const std::string& directoryPath
    );
    void createTexture(
        Material& material,
        const FbxSurfaceMaterial* fbxSurfaceMaterial,
        const std::string& directoryPath,
        const char* type
    );
    FbxFileTexture* getFbxTexture(const FbxProperty& prop) const;

    //ボーン読み込み
    void loadBone(MeshVertices& meshVertices, FbxMesh* fbxMesh);
    //ウェイト読み込み
    void loadWeight(MeshVertices& meshVertices, const FbxMesh* fbxMesh, const FbxCluster* bone, unsigned boneIndex);
    //頂点ウェイトを正規化する
    void normalizeWeight(MeshVertices& meshVertices);
    //キーフレーム読み込み
    void loadKeyFrames(const std::string& name, int boneIndex, FbxNode* fbxBoneNode);

    void time(FbxMesh* fbxMesh);

private:
    //ボーン情報
    std::vector<Bone> mBones;
    std::vector<std::string> mAnimationNames;
    Motion mMotion;
    int mStartFrame;
    int mEndFrame;
};
