#pragma once

#include "../IMeshLoader.h"
#include "../../Math/Math.h"
#include <fbxsdk.h>
#include <memory>
#include <string>
#include <vector>

//ボーン構造体
struct Bone {
    //ボーン名
    std::string name;
    //フレーム数
    unsigned numFrame;
    //初期姿勢
    Matrix4 initMat;
    //フレーム時姿勢
    std::vector<Matrix4> frameMat;
};

class FbxAnimationTime;

class FbxBoneParser {
public:
    FbxBoneParser();
    ~FbxBoneParser();

    //ボーンを解析する
    void parse(MeshVertices& meshVertices, FbxMesh* fbxMesh);

private:
    //ボーン読み込み
    void loadBone(MeshVertices& meshVertices, const FbxMesh* fbxMesh, FbxSkin* fbxSkin);
    //ウェイト読み込み
    void loadWeight(MeshVertices& meshVertices, const FbxMesh* fbxMesh, const FbxCluster* bone, unsigned boneIndex);
    //頂点ウェイトを正規化する
    void normalizeWeight(MeshVertices& meshVertices);
    //キーフレーム読み込み
    void loadKeyFrames(Bone& bone, FbxCluster* fbxCluster);
    //FbxMatirxからMatrix4へ変換する
    Matrix4 substitutionMatrix(const FbxMatrix& src) const;

private:
    //ボーン情報
    std::unique_ptr<FbxAnimationTime> mAnimationTime;
    std::vector<Bone> mBones;
};
