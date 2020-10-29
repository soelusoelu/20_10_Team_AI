#include "FbxBoneParser.h"
#include "FbxAnimationTime.h"

FbxBoneParser::FbxBoneParser() :
    mAnimationTime(std::make_unique<FbxAnimationTime>()) {
}

FbxBoneParser::~FbxBoneParser() = default;

void FbxBoneParser::parse(MeshVertices& meshVertices, FbxMesh* fbxMesh) {
    mAnimationTime->parse(fbxMesh->GetScene());

    //スキン情報の有無
    int skinCount = fbxMesh->GetDeformerCount(FbxDeformer::eSkin);
    if (skinCount <= 0) {
        return;
    }

    for (int i = 0; i < skinCount; ++i) {
        //i番目のスキンを取得
        FbxDeformer* fbxDeformer = fbxMesh->GetDeformer(i);
        //FbxDeformer* fbxDeformer = fbxMesh->GetDeformer(i, FbxDeformer::eSkin);
        FbxSkin* fbxSkin = static_cast<FbxSkin*>(fbxDeformer);

        //ボーンを読み込んでいく
        loadBone(meshVertices, fbxMesh, fbxSkin);
    }

    //ウェイト正規化
    normalizeWeight(meshVertices);
}

void FbxBoneParser::loadBone(MeshVertices& meshVertices, const FbxMesh* fbxMesh, FbxSkin* fbxSkin) {
    //ボーンの数を取得
    int boneCount = fbxSkin->GetClusterCount();
    if (boneCount <= 0) {
        return;
    }

    //ボーンの数に合わせて拡張
    mBones.resize(boneCount);

    for (int i = 0; i < boneCount; ++i) {
        //j番目のボーンを取得
        FbxCluster* bone = fbxSkin->GetCluster(i);

        //ウェイト読み込み
        loadWeight(meshVertices, fbxMesh, bone, i);

        //キーフレーム読み込み
        loadKeyFrames(mBones[i], bone);
    }
}

void FbxBoneParser::loadWeight(MeshVertices& meshVertices, const FbxMesh* fbxMesh, const FbxCluster* bone, unsigned boneIndex) {
    //影響する頂点の数
    int weightCount = bone->GetControlPointIndicesCount();
    //このボーンによって移動する頂点のインデックスの配列
    int* weightIndices = bone->GetControlPointIndices();
    //重み
    double* weights = bone->GetControlPointWeights();
    //頂点のインデックス
    int* polygonVertices = fbxMesh->GetPolygonVertices();

    for (int i = 0; i < weightCount; ++i) {
        int index = weightIndices[i];
        //全ポリゴンからindex2番目の頂点検索
        for (int j = 0; j < meshVertices.size(); ++j) {
            //頂点番号と一致するのを探す
            if (polygonVertices[j] != index) {
                continue;
            }

            //頂点にウェイト保存
            int weightCount;
            for (weightCount = 0; weightCount < 4; ++weightCount) {
                //ウェイトが0なら格納できる
                if (Math::nearZero(meshVertices[j].weight[weightCount])) {
                    break;
                }
            }

            //格納できる数を超えていたら
            if (weightCount >= 4) {
                continue;
            }

            //頂点情報にウェイトを追加
            meshVertices[j].index[weightCount] = boneIndex;
            meshVertices[j].weight[weightCount] = static_cast<float>(weights[i]);
        }
    }
}

void FbxBoneParser::normalizeWeight(MeshVertices& meshVertices) {
    //5本以上にまたっがてる場合のため
    for (int i = 0; i < meshVertices.size(); ++i) {
        auto& meshVertex = meshVertices[i];
        float sumWeight = 0.f;
        //頂点のウェイトの合計を求める
        for (int j = 0; j < 4; ++j) {
            if (meshVertex.weight[j] <= 0.f) {
                break;
            }
            sumWeight += meshVertex.weight[j];
        }
        //正規化
        for (int j = 0; j < 4; ++j) {
            meshVertex.weight[j] /= sumWeight;
        }
    }
}

void FbxBoneParser::loadKeyFrames(Bone& bone, FbxCluster* fbxCluster) {
    //ノードを取得
    FbxNode* fbxNode = fbxCluster->GetLink();

    //ボーン名取得
    bone.name = fbxNode->GetName();

    //フレーム数を取得
    bone.numFrame = mAnimationTime->mStopFrame;

    //モデルの初期姿勢を取得する
    FbxAMatrix linkMatrix;
    fbxCluster->GetTransformLinkMatrix(linkMatrix);
    bone.initMat = substitutionMatrix(linkMatrix);

    //フレーム数分拡張しとく
    bone.frameMat.resize(bone.numFrame);

    //フレーム数分フレーム時姿勢を取得する
    for (int j = 0; j < bone.numFrame; ++j) {
        //指定フレームでの時間を取得する
        auto time = mAnimationTime->getTime(j);
        //指定フレームでの姿勢
        bone.frameMat[j] = substitutionMatrix(fbxNode->EvaluateGlobalTransform(time));
    }
}

Matrix4 FbxBoneParser::substitutionMatrix(const FbxMatrix& src) const {
    Matrix4 dst;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            dst.m[i][j] = static_cast<float>(src.mData[i][j]);
        }
    }

    return dst;
}
