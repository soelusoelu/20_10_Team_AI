#pragma once

#include "../IMeshLoader.h"
#include <fbxsdk.h>
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
    std::string name;
    long long numFrame; //フレーム数
    long long startFrame;
    long long endFrame;
    //[ボーンのインデックス][フレーム数]
    std::vector<Matrix4> key[512]; //キーフレーム
};

class FbxBoneParser {
public:
    FbxBoneParser();
    ~FbxBoneParser();

    //ボーンを解析する
    void parse(MeshVertices& meshVertices, FbxMesh* fbxMesh);
    //モーション読み込み
    void loadMotion(FbxScene* fbxScene);

private:
    //ウェイト読み込み
    void loadWeight(MeshVertices& meshVertices, const FbxMesh* fbxMesh, const FbxCluster* bone, unsigned boneIndex);
    //頂点ウェイトを正規化する
    void normalizeWeight(MeshVertices& meshVertices);
    //キーフレーム読み込み
    void loadKeyFrames(const std::string& name, int boneIndex, FbxNode* fbxBoneNode);
    //モーション名からモーションを検索する
    int getMotionFromName(const std::string& motionName) const;

private:
    //ボーン情報
    std::vector<Bone> mBones;
    std::vector<Motion> mMotions;
};
