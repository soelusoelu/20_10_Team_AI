#include "FbxBoneParser.h"

FbxBoneParser::FbxBoneParser() = default;

FbxBoneParser::~FbxBoneParser() = default;

void FbxBoneParser::parse(MeshVertices& meshVertices, FbxMesh* fbxMesh) {
    //スキン情報の有無
    int skinCount = fbxMesh->GetDeformerCount(FbxDeformer::eSkin);
    if (skinCount <= 0) {
        return;
    }

    for (int i = 0; i < skinCount; ++i) {
        //i番目のスキンを取得
        FbxDeformer* fbxDeformer = fbxMesh->GetDeformer(i, FbxDeformer::eSkin);
        FbxSkin* fbxSkin = static_cast<FbxSkin*>(fbxDeformer);

        //ボーンの数を取得
        int boneCount = fbxSkin->GetClusterCount();

        //ボーンの数に合わせて拡張
        mBones.resize(boneCount);

        for (int j = 0; j < boneCount; ++j) {
            //j番目のボーンを取得
            FbxCluster* bone = fbxSkin->GetCluster(j);

            //ボーンのインデックスを格納
            //mBones[j].index = j;

            //ノードを取得
            FbxNode* fbxNode = bone->GetLink();

            //ボーン名取得
            mBones[j].name = fbxNode->GetName();

            //ウェイト読み込み
            loadWeight(meshVertices, fbxMesh, bone, j);

            //モデルの初期姿勢を取得する
            FbxAMatrix linkMatrix;
            bone->GetTransformLinkMatrix(linkMatrix);

            //キーフレーム読み込み
            //mMotions[0].key[j].resize(boneCount);
            //loadKeyFrames(mMotions[0].name, j, fbxNode);
        }
    }

    //ウェイト正規化
    normalizeWeight(meshVertices);



    //全ボーン情報取得
    //for (int bone = 0; bone < boneCount; ++bone) {
    //    //ボーン情報取得
    //    FbxCluster* fbxCluster = fbxSkin->GetCluster(bone);
    //    FbxAMatrix trans;
    //    fbxCluster->GetTransformMatrix(trans);
    //    trans.mData[0][1] *= -1;
    //    trans.mData[0][2] *= -1;
    //    trans.mData[1][0] *= -1;
    //    trans.mData[2][0] *= -1;
    //    trans.mData[3][0] *= -1;

    //    //ボーン名取得
    //    const char* pName = fbxCluster->GetLink()->GetName();
    //    mBones[bone].name = pName;

    //    //Bone* pBone;
    //    ////ボーン検索
    //    //int boneNo = findBone(pName);
    //    //if (boneNo != -1) {
    //    //    pBone = &mBones[boneNo];
    //    //} else {
    //    //    boneNo = mBones.size();
    //    //    pBone = &mBones[boneNo];
    //    //    ++mNumBone;

    //    //    pBone->name = pName;
    //    //    //オフセット行列作成
    //    //    FbxAMatrix linkMatrix;
    //    //    fbxCluster->GetTransformLinkMatrix(linkMatrix);
    //    //    linkMatrix.mData[0][1] *= -1;
    //    //    linkMatrix.mData[0][2] *= -1;
    //    //    linkMatrix.mData[1][0] *= -1;
    //    //    linkMatrix.mData[2][0] *= -1;
    //    //    linkMatrix.mData[3][0] *= -1;

    //    //    FbxAMatrix offset = linkMatrix.Inverse() * trans;

    //    //    //オフセット行列保存
    //    //    //FbxAMatrixがDouble型だからmemcpyできない
    //    //    for (int i = 0; i < 4; ++i) {
    //    //        for (int j = 0; j < 4; ++j) {
    //    //            pBone->offset.m[i][j] = static_cast<float>(offset[i][j]);
    //    //        }
    //    //    }
    //    //    //キーフレーム読み込み
    //    //    loadKeyFrames("default", boneNo, pCluster->GetLink());
    //    //}
    //}
}

void FbxBoneParser::loadMotion(FbxScene* fbxScene) {
    //モーション情報取得
    FbxArray<FbxString*> animationNames;
    fbxScene->FillAnimStackNameArray(animationNames);

    mMotions.resize(animationNames.Size());
    for (int i = 0; i < animationNames.Size(); i++) {
        mMotions[i].name = (*animationNames[i]);
    }

    FbxLongLong start;
    FbxLongLong stop;
    for (int i = 0; i < animationNames.Size(); ++i) {
        FbxTakeInfo* currentTakeInfo = fbxScene->GetTakeInfo(*animationNames[i]);
        if (currentTakeInfo) {
            start = currentTakeInfo->mLocalTimeSpan.GetStart().Get();
            stop = currentTakeInfo->mLocalTimeSpan.GetStop().Get();

            //アニメーションの開始と終了の時間
            mMotions[i].startFrame = static_cast<long long>(start);
            mMotions[i].endFrame = static_cast<long long>(stop);
            //フレーム数を求める
            mMotions[i].numFrame = static_cast<long long>((stop - start) / FbxTime::GetOneFrameValue(FbxTime::EMode::eFrames60));
        }
    }

    //FbxArrayを削除する
    FbxArrayDelete(animationNames);
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

void FbxBoneParser::loadKeyFrames(const std::string& name, int boneIndex, FbxNode* fbxBoneNode) {
    //モーション名からモーションのインデックスを取得する
    int motionID = getMotionFromName(name);
    if (motionID < 0) {
        return;
    }

    auto& motion = mMotions[motionID];
    double time = motion.startFrame * (1.0 / 60.0);
    const unsigned numFrame = static_cast<unsigned>(motion.numFrame);
    FbxTime fbxTime;
    for (unsigned f = 0; f < numFrame; ++f) {
        fbxTime.SetSecondDouble(time);
        //T秒の姿勢行列をGet
        FbxMatrix m = fbxBoneNode->EvaluateGlobalTransform(fbxTime);
        m.mData[0][1] *= -1;
        m.mData[0][2] *= -1;
        m.mData[1][0] *= -1;
        m.mData[2][0] *= -1;
        m.mData[3][0] *= -1;

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                motion.key[boneIndex][f].m[i][j] = static_cast<float>(m[i][j]);
            }
        }

        time += 1.0 / 60.0;
    }
}

int FbxBoneParser::getMotionFromName(const std::string& motionName) const {
    for (int i = 0; i < mMotions.size(); ++i) {
        if (mMotions[i].name == motionName) {
            return i;
        }
    }

    //名前に一致するモーションが見つからない
    return -1;
}
