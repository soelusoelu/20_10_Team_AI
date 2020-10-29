#pragma once

#include "../Bone.h"
#include "../IMeshLoader.h"
#include "../../Math/Math.h"
#include <fbxsdk.h>
#include <memory>
#include <vector>

class FbxAnimationTime;

//FBXボーン解析クラス
class FbxBoneParser {
public:
    FbxBoneParser();
    ~FbxBoneParser();

    //ボーンを解析する
    void parse(MeshVertices& meshVertices, std::vector<Bone>& bones, FbxMesh* fbxMesh);

private:
    //ボーン読み込み
    void loadBone(MeshVertices& meshVertices, std::vector<Bone>& bones, const FbxMesh* fbxMesh, FbxSkin* fbxSkin);
    //ウェイト読み込み
    void loadWeight(MeshVertices& meshVertices, const FbxMesh* fbxMesh, const FbxCluster* bone, unsigned boneIndex);
    //頂点ウェイトを正規化する
    void normalizeWeight(MeshVertices& meshVertices);
    //キーフレーム読み込み
    void loadKeyFrames(Bone& bone, FbxCluster* fbxCluster);
    //FbxMatirxからMatrix4へ変換する
    Matrix4 substitutionMatrix(const FbxMatrix& src) const;

private:
    std::unique_ptr<FbxAnimationTime> mAnimationTime;
};
