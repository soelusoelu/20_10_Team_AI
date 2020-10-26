﻿#include "FBX.h"
#include "Material.h"
#include "../DebugLayer/Debug.h"
#include "../System/AssetsManager.h"
#include "../Utility/FileUtil.h"

FBX::FBX() = default;

FBX::~FBX() = default;

void FBX::perse(
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
    loadMaterial(material, fbxMesh, directoryPath);
}

void FBX::loadNormal(FbxMesh* mesh) {
    //auto& normals = mNormals[meshIndex];

    ////法線配列を取得する
    //FbxArray<FbxVector4> normalArray;
    //mesh->GetPolygonVertexNormals(normalArray);
    //normals.resize(normalArray.Size());

    //for (size_t i = 0; i < normals.size(); i++) {
    //    normals[i].x = -static_cast<float>(normalArray[i][0]);
    //    normals[i].y = static_cast<float>(normalArray[i][1]);
    //    normals[i].z = static_cast<float>(normalArray[i][2]);
    //}

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
    //auto& uvs = mUVs[meshIndex];

    ////UVSetの名前リストを取得
    //FbxStringList uvSetNameList;
    //mesh->GetUVSetNames(uvSetNameList);

    ////UVSetの名前からUVSetを取得する
    //FbxArray<FbxVector2> uvArray;
    //mesh->GetPolygonVertexUVs(uvSetNameList.GetStringAt(0), uvArray);
    //uvs.resize(uvArray.Size());

    //for (size_t i = 0; i < uvs.size(); i++) {
    //    uvs[i].x = static_cast<float>(uvArray[i][0]);
    //    uvs[i].y = 1.f - static_cast<float>(uvArray[i][1]);
    //}

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
        if (tangents) {
            const auto& tangent = *tangents;
            vertex.tangent.x = tangent[i][0];
            vertex.tangent.y = tangent[i][1];
            vertex.tangent.z = tangent[i][2];
        }

        //頂点情報を格納
        meshVertices[i] = vertex;

        //fbxは右手系なので、DirectXの左手系に直すために2->1->0の順にインデックスを格納していく
        indices[i * 3 + 0] = i * 3 + 2;
        indices[i * 3 + 1] = i * 3 + 1;
        indices[i * 3 + 2] = i * 3;
    }
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

void FBX::loadMaterial(
    Material& material,
    FbxMesh* fbxMesh,
    const std::string& directoryPath
) {
    //マテリアルがなければ終了
    if (fbxMesh->GetElementMaterialCount() == 0) {
        return;
    }

    //Mesh側のマテリアル情報を取得
    FbxLayerElementMaterial* fbxMaterial = fbxMesh->GetElementMaterial(0);

    //インデックスバッファからインデックスを取得
    int index = fbxMaterial->GetIndexArray().GetAt(0);
    //マテリアル取得
    FbxSurfaceMaterial* surfaceMaterial = fbxMesh->GetNode()->GetSrcObject<FbxSurfaceMaterial>(index);

    //マテリアル名の保存
    if (surfaceMaterial) {
        //マテリアル名を取得
        material.materialName = surfaceMaterial->GetName();

        //テクスチャ以外の情報を取得
        loadMaterialAttribute(material, surfaceMaterial);

        //テクスチャを取得
        loadTextures(material, surfaceMaterial, directoryPath);
    }
}

void FBX::loadMaterialAttribute(Material& material, FbxSurfaceMaterial* fbxSurfaceMaterial) {
    //継承関係にあるか調べる
    if (!fbxSurfaceMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId)) {
        return;
    }

    FbxSurfaceLambert* lambert = static_cast<FbxSurfaceLambert*>(fbxSurfaceMaterial);
    if (!lambert) {
        return;
    }

    //まずはランバート分読み込む
    loadLambert(material, lambert);

    //継承関係にあるか調べる
    if (lambert->GetClassId().Is(FbxSurfacePhong::ClassId)) {
        return;
    }

    FbxSurfacePhong* phong = static_cast<FbxSurfacePhong*>(lambert);
    if (!phong) {
        return;
    }

    //Phongだったら追加読み込み
    loadPhong(material, phong);
}

void FBX::loadLambert(Material& material, const FbxSurfaceLambert* fbxSurfaceLambert) {
    //アンビエント
    auto prop = fbxSurfaceLambert->FindProperty(FbxSurfaceMaterial::sAmbient);
    if (prop.IsValid()) {
        const auto& ambient = fbxSurfaceLambert->Ambient.Get();
        material.ambient.x = static_cast<float>(ambient[0]);
        material.ambient.y = static_cast<float>(ambient[1]);
        material.ambient.z = static_cast<float>(ambient[2]);
    }

    //ディヒューズ
    prop = fbxSurfaceLambert->FindProperty(FbxSurfaceMaterial::sDiffuse);
    if (prop.IsValid()) {
        const auto& diffuse = fbxSurfaceLambert->Diffuse.Get();
        material.diffuse.x = static_cast<float>(diffuse[0]);
        material.diffuse.y = static_cast<float>(diffuse[1]);
        material.diffuse.z = static_cast<float>(diffuse[2]);
    }

    //エミッシブ
    prop = fbxSurfaceLambert->FindProperty(FbxSurfaceMaterial::sEmissive);
    if (prop.IsValid()) {
        const auto& emissive = fbxSurfaceLambert->Emissive.Get();
        material.emissive.x = static_cast<float>(emissive[0]);
        material.emissive.y = static_cast<float>(emissive[1]);
        material.emissive.z = static_cast<float>(emissive[2]);
    }

    //バンプ
    prop = fbxSurfaceLambert->FindProperty(FbxSurfaceMaterial::sBump);
    if (prop.IsValid()) {
        const auto& bump = fbxSurfaceLambert->Bump.Get();
        material.bump.x = static_cast<float>(bump[0]);
        material.bump.y = static_cast<float>(bump[1]);
        material.bump.z = static_cast<float>(bump[2]);
    }

    //透過度
    prop = fbxSurfaceLambert->FindProperty(FbxSurfaceMaterial::sTransparencyFactor);
    if (prop.IsValid()) {
        material.transparency = static_cast<float>(fbxSurfaceLambert->TransparencyFactor.Get());
    }
}

void FBX::loadPhong(Material& material, const FbxSurfacePhong* fbxSurfacePhong) {
    //スペキュラ
    auto prop = fbxSurfacePhong->FindProperty(FbxSurfaceMaterial::sSpecular);
    if (prop.IsValid()) {
        const auto& specular = fbxSurfacePhong->Specular.Get();
        material.specular.x = static_cast<float>(specular[0]);
        material.specular.y = static_cast<float>(specular[1]);
        material.specular.z = static_cast<float>(specular[2]);
    }

    //光沢
    prop = fbxSurfacePhong->FindProperty(FbxSurfaceMaterial::sShininess);
    if (prop.IsValid()) {
        material.shininess = static_cast<float>(fbxSurfacePhong->Shininess.Get());
    }
}

void FBX::loadTextures(
    Material& material,
    FbxSurfaceMaterial* fbxSurfaceMaterial,
    const std::string& directoryPath
) {
    //テクスチャ作成
    createTexture(material, fbxSurfaceMaterial, directoryPath, FbxSurfaceMaterial::sDiffuse);
    createTexture(material, fbxSurfaceMaterial, directoryPath, FbxSurfaceMaterial::sNormalMap);
}

void FBX::createTexture(
    Material& material,
    const FbxSurfaceMaterial* fbxSurfaceMaterial,
    const std::string& directoryPath,
    const char* type
) {
    //プロパティを取得する
    const auto& prop = fbxSurfaceMaterial->FindProperty(type);

    //テクスチャを取得する
    auto fbxTexture = getFbxTexture(prop);

    //テクスチャを使用していなかったら終了
    if (!fbxTexture) {
        return;
    }

    //ファイルパスを相対パスで取得
    auto filePath = fbxTexture->GetRelativeFileName();
    //ファイルパスからファイル名を取得する
    const auto& textureName = FileUtil::getFileNameFromDirectry(filePath);

    //ファイル名からテクスチャを作成する
    const auto& tex = AssetsManager::instance().createTexture(textureName, directoryPath);

    //指定されたテクスチャに渡す
    if (type == FbxSurfaceMaterial::sDiffuse) {
        material.texture = tex;
    } else if (type == FbxSurfaceMaterial::sNormalMap) {
        material.mapTexture = tex;
    }
}

FbxFileTexture* FBX::getFbxTexture(const FbxProperty& prop) const {
    FbxFileTexture* out = nullptr;

    int numTexture = prop.GetSrcObjectCount<FbxFileTexture>();
    if (numTexture > 0) {
        out = prop.GetSrcObject<FbxFileTexture>(0);
    } else {
        //失敗したらマルチテクスチャの可能性を考えて、FbxLayerdTextureを指定する
        //FbxLayerdTextureからテクスチャを取得する
        int numLayer = prop.GetSrcObjectCount<FbxLayeredTexture>();
        if (numLayer > 0) {
            out = prop.GetSrcObject<FbxFileTexture>(0);
        }
    }

    return out;
}
