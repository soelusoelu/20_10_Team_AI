#include "FBX.h"
#include "FbxMaterialParser.h"
#include "../Material.h"
#include "../../DebugLayer/Debug.h"
#include "../../System/AssetsManager.h"
#include "../../Utility/FileUtil.h"

FBX::FBX() :
    mMaterialParser(std::make_unique<FbxMaterialParser>()) {
}

FBX::~FBX() = default;

void FBX::parse(
    const std::string& filePath,
    std::vector<MeshVertices>& meshesVertices,
    std::vector<Indices>& meshesIndices,
    std::vector<Material>& materials
) {
    //マネージャーを生成
    auto manager = FbxManager::Create();

    //IOSettingを生成
    FbxIOSettings* ioSettings = FbxIOSettings::Create(manager, IOSROOT);
    manager->SetIOSettings(ioSettings);

    //Importerを生成
    FbxImporter* importer = FbxImporter::Create(manager, "");
    if (!importer->Initialize(filePath.c_str(), -1, manager->GetIOSettings())) {
        Debug::windowMessage(filePath + ": ファイルは存在しません");
        return;
    }

    //SceneオブジェクトにFBXファイル内の情報を流し込む
    FbxScene* scene = FbxScene::Create(manager, "scene");
    importer->Import(scene);
    importer->Destroy(); //シーンを流し込んだらImporterは解放していい

    //四角ポリゴンを三角ポリゴンに変換
    FbxGeometryConverter geometryConverter(manager);
    geometryConverter.Triangulate(scene, true);

    //マテリアル単位でメッシュを分割する
    geometryConverter.SplitMeshesPerMaterial(scene, true);

    //モーションを読み込む
    loadMotion(scene);

    //FbxMeshの数を取得
    auto numMeshes = scene->GetSrcObjectCount<FbxMesh>();

    //メッシュの数に合わせて拡張する
    meshesVertices.resize(numMeshes);
    meshesIndices.resize(numMeshes);
    materials.resize(numMeshes);

    //ファイルパスからディレクトリパスを抜き出す
    auto directoryPath = FileUtil::getDirectryFromFilePath(filePath);

    //FbxMeshの数だけメッシュを作成する
    for (size_t i = 0; i < numMeshes; ++i) {
        auto mesh = scene->GetSrcObject<FbxMesh>(i);
        createMesh(
            meshesVertices[i],
            meshesIndices[i],
            materials[i],
            mesh,
            directoryPath
        );
    }

    //マネージャー解放
    manager->Destroy();
}

void FBX::createMesh(
    MeshVertices& meshVertices,
    Indices& indices,
    Material& material,
    FbxMesh* fbxMesh,
    const std::string& directoryPath
) {
    loadFace(meshVertices, indices, fbxMesh);
    //マテリアルの読み込み
    mMaterialParser->parse(material, fbxMesh, directoryPath);
    loadBone(meshVertices, fbxMesh);
}

void FBX::loadNormal(FbxMesh* mesh) {
#pragma region 公式サンプル
    //FbxGeometryElementNormal* normalElement = mesh->GetElementNormal();
    //if (!normalElement) {
    //    return;
    //}

    //if (normalElement->GetMappingMode() == FbxGeometryElement::eByControlPoint) {
    //    for (size_t i = 0; i < mesh->GetControlPointsCount(); i++) {
    //        int normalIndex = 0;
    //        if (normalElement->GetReferenceMode() == FbxGeometryElement::eDirect) {
    //            normalIndex = i;
    //        }
    //        if (normalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect) {
    //            normalIndex = normalElement->GetIndexArray().GetAt(i);
    //        }

    //        FbxVector4 normal = normalElement->GetDirectArray().GetAt(normalIndex);
    //        mNormals[normalIndex].x = static_cast<float>(-normal[0]);
    //        mNormals[normalIndex].y = static_cast<float>(normal[1]);
    //        mNormals[normalIndex].z = static_cast<float>(normal[2]);
    //    }
    //} else if (normalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {
    //    int indexByPolygonVertex = 0;
    //    for (size_t i = 0; i < mesh->GetPolygonCount(); i++) {
    //        int polygonSize = mesh->GetPolygonSize(i);
    //        for (size_t j = 0; j < polygonSize; j++) {
    //            int normalIndex = 0;
    //            if (normalElement->GetReferenceMode() == FbxGeometryElement::eDirect) {
    //                normalIndex = indexByPolygonVertex;
    //            }
    //            if (normalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect) {
    //                normalIndex = normalElement->GetIndexArray().GetAt(indexByPolygonVertex);
    //            }

    //            FbxVector4 normal = normalElement->GetDirectArray().GetAt(normalIndex);
    //            mNormals[normalIndex].x = static_cast<float>(-normal[0]);
    //            mNormals[normalIndex].y = static_cast<float>(normal[1]);
    //            mNormals[normalIndex].z = static_cast<float>(normal[2]);

    //            indexByPolygonVertex++;
    //        }
    //    }
    //}
#pragma endregion
}

void FBX::loadUV(FbxMesh* mesh) {
#pragma region 公式サンプル
    //すべてのUVセットを反復処理する
    //for (int uvSetIndex = 0; uvSetIndex < uvSetNameList.GetCount(); uvSetIndex++) {
    //    //uvSetIndex-番目のUVセットを取得
    //    const char* uvSetName = uvSetNameList.GetStringAt(uvSetIndex);
    //    const FbxGeometryElementUV* uvElement = mesh->GetElementUV(uvSetName);
    //    if (!uvElement) {
    //        continue;
    //    }

    //    //マッピングモードeByPolygonVertexおよびeByControlPointのみをサポート
    //    if (uvElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex && uvElement->GetMappingMode() != FbxGeometryElement::eByControlPoint) {
    //        return;
    //    }

    //    //インデックス配列。uvデータを参照するインデックスを保持します
    //    const bool useIndex = uvElement->GetReferenceMode() != FbxGeometryElement::eDirect;
    //    const int indexCount = (useIndex) ? uvElement->GetIndexArray().GetCount() : 0;

    //    //ポリゴンごとにデータを反復処理する
    //    const int polyCount = mesh->GetPolygonCount();

    //    if (uvElement->GetMappingMode() == FbxGeometryElement::eByControlPoint) {
    //        for (int polyIndex = 0; polyIndex < polyCount; polyIndex++) {
    //            //MakePolyに渡す必要がある最大インデックス配列を作成します
    //            const int polySize = mesh->GetPolygonSize(polyIndex);
    //            for (int vertIndex = 0; vertIndex < polySize; vertIndex++) {
    //                //頂点配列の現在の頂点のインデックスを取得します
    //                int polyVertIndex = mesh->GetPolygonVertex(polyIndex, vertIndex);

    //                //UVインデックスは参照モードに依存します
    //                int uvIndex = useIndex ? uvElement->GetIndexArray().GetAt(polyVertIndex) : polyVertIndex;

    //                FbxVector2 uv = uvElement->GetDirectArray().GetAt(uvIndex);
    //                mUVs[uvIndex].x = static_cast<float>(uv[0]);
    //                mUVs[uvIndex].y = 1.f - static_cast<float>(uv[1]);
    //            }
    //        }
    //    } else if (uvElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {
    //        int polyIndexCounter = 0;
    //        for (int polyIndex = 0; polyIndex < polyCount; polyIndex++) {
    //            //MakePolyに渡す必要がある最大インデックス配列を作成します
    //            const int polySize = mesh->GetPolygonSize(polyIndex);
    //            for (int vertIndex = 0; vertIndex < polySize; vertIndex++) {
    //                if (polyIndexCounter < indexCount) {
    //                    //UVインデックスは参照モードに依存します
    //                    int uvIndex = useIndex ? uvElement->GetIndexArray().GetAt(polyIndexCounter) : polyIndexCounter;

    //                    FbxVector2 uv = uvElement->GetDirectArray().GetAt(uvIndex);
    //                    mUVs[uvIndex].x = static_cast<float>(uv[0]);
    //                    mUVs[uvIndex].y = 1.f - static_cast<float>(uv[1]);

    //                    polyIndexCounter++;
    //                }
    //            }
    //        }
    //    }
    //}
#pragma endregion
}

void FBX::computeIndices(Indices& indices, FbxMesh* fbxMesh) {
    //indicesはポリゴン数の3倍
    auto polygonCount = fbxMesh->GetPolygonCount();
    indices.resize(polygonCount * 3);

    //Fbxは右手系なので、DirectXの左手系に直すために2->1->0の順にインデックスを格納していく
    for (int i = 0; i < polygonCount; ++i) {
        indices[i * 3 + 0] = i * 3 + 2;
        indices[i * 3 + 1] = i * 3 + 1;
        indices[i * 3 + 2] = i * 3;
    }
}

void FBX::loadFace(
    MeshVertices& meshVertices,
    Indices& indices,
    FbxMesh* fbxMesh
) {
    //頂点数
    auto polygonVertexCount = fbxMesh->GetPolygonVertexCount();
    //インデックスバッファの取得
    int* polygonVertices = fbxMesh->GetPolygonVertices();
    //頂点座標配列
    FbxVector4* src = fbxMesh->GetControlPoints();

    //法線配列を取得する
    FbxArray<FbxVector4> normalArray;
    fbxMesh->GetPolygonVertexNormals(normalArray);

    //UVSetの名前リストを取得
    FbxStringList uvSetNameList;
    fbxMesh->GetUVSetNames(uvSetNameList);
    //UVSetの名前からUVSetを取得する
    FbxArray<FbxVector2> uvArray;
    fbxMesh->GetPolygonVertexUVs(uvSetNameList.GetStringAt(0), uvArray);

    //UVSetの名前からTangent(接線)を生成する
    fbxMesh->GenerateTangentsData(uvSetNameList.GetStringAt(0));
    FbxLayerElementArrayTemplate<FbxVector4>* tangents;
    //生成後、取得する
    fbxMesh->GetTangents(&tangents);

    //事前に拡張しとく
    meshVertices.resize(polygonVertexCount);

    //indicesは頂点数の3倍
    indices.resize(polygonVertexCount * 3);

    for (size_t i = 0; i < polygonVertexCount; ++i) {
        MeshVertex vertex;

        int index = polygonVertices[i];
        vertex.pos.x = static_cast<float>(-src[index][0]);
        vertex.pos.y = static_cast<float>(src[index][1]);
        vertex.pos.z = static_cast<float>(src[index][2]);

        vertex.normal.x = static_cast<float>(-normalArray[i][0]);
        vertex.normal.y = static_cast<float>(normalArray[i][1]);
        vertex.normal.z = static_cast<float>(normalArray[i][2]);

        //UVは使用している場合のみ
        if (uvArray.Size() > 0) {
            vertex.uv.x = static_cast<float>(uvArray[i][0]);
            vertex.uv.y = 1.f - static_cast<float>(uvArray[i][1]);
        }

        //タンジェントは上手く生成できた場合のみ
        //if (tangents) {
        //    const auto& tangent = *tangents;
        //    vertex.tangent.x = tangent[i][0];
        //    vertex.tangent.y = tangent[i][1];
        //    vertex.tangent.z = tangent[i][2];
        //}

        //頂点情報を格納
        meshVertices[i] = vertex;

        //fbxは右手系なので、DirectXの左手系に直すために2->1->0の順にインデックスを格納していく
        indices[i * 3 + 0] = i * 3 + 2;
        indices[i * 3 + 1] = i * 3 + 1;
        indices[i * 3 + 2] = i * 3;
    }
}

void FBX::loadBone(MeshVertices& meshVertices, FbxMesh* fbxMesh) {
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

void FBX::loadWeight(MeshVertices& meshVertices, const FbxMesh* fbxMesh, const FbxCluster* bone, unsigned boneIndex) {
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

void FBX::normalizeWeight(MeshVertices& meshVertices) {
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

void FBX::loadKeyFrames(const std::string& name, int boneIndex, FbxNode* fbxBoneNode) {
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

void FBX::loadMotion(FbxScene* fbxScene) {
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
}

int FBX::getMotionFromName(const std::string& motionName) const {
    for (int i = 0; i < mMotions.size(); ++i) {
        if (mMotions[i].name == motionName) {
            return i;
        }
    }

    //名前に一致するモーションが見つからない
    return -1;
}
