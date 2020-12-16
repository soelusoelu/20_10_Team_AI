#include "FbxMeshParser.h"
#include "FbxUtility.h"
#include <cassert>

FbxMeshParser::FbxMeshParser() = default;

FbxMeshParser::~FbxMeshParser() = default;

//長いけど無駄にfor文を回さなくて済む
void FbxMeshParser::parse(
    MeshVertices & meshVertices,
    Indices & indices,
    FbxMesh * fbxMesh
) const {
    //頂点数
    int vertexCount = fbxMesh->GetControlPointsCount();
    //頂点座標配列
    FbxVector4* src = fbxMesh->GetControlPoints();

    //事前に拡張しとく
    meshVertices.resize(vertexCount);

    //インデックスバッファ取得
    getIndices(indices, fbxMesh);

    //法線とUVを取得
    Normals normals;
    UVs uvs;
    getNormalsAndUVs(normals, uvs, indices, fbxMesh);

    //メッシュごとのトランスフォームを計算
    FbxNode* node = fbxMesh->GetNode();
    auto t = FbxUtility::fbxDouble3ToVector3(node->LclTranslation.Get());
    auto r = FbxUtility::fbxDouble3ToVector3(node->LclRotation.Get());
    Quaternion q(r);
    auto s = FbxUtility::fbxDouble3ToVector3(node->LclScaling.Get());
    auto mat = Matrix4::createScale(s) * Matrix4::createFromQuaternion(q) * Matrix4::createTranslation(t);

    for (size_t i = 0; i < vertexCount; ++i) {
        MeshVertex vertex;

        vertex.pos = FbxUtility::fbxVector4ToVector3(src[i]);
        vertex.pos.x *= -1.f;
        vertex.pos = Vector3::transform(vertex.pos, mat);

        vertex.normal = Vector3::transform(normals[i], q);

        if (uvs.size() > 0) {
            vertex.uv = uvs[i];
        }

        //頂点情報を格納
        meshVertices[i] = vertex;
    }
}

void FbxMeshParser::loadNormal(FbxMesh * mesh) {
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
}

void FbxMeshParser::loadUV(FbxMesh * mesh) {
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
}

void FbxMeshParser::getIndices(
    Indices & indices,
    const FbxMesh * fbxMesh
) const {
    //ポリゴン頂点数
    int polygonVertexCount = fbxMesh->GetPolygonVertexCount();
    //for文をループする回数
    int loopCount = polygonVertexCount / 3;

    //ポリゴンの頂点数分拡張する
    indices.resize(polygonVertexCount);

    for (int i = 0; i < loopCount; ++i) {
        //fbxは右手系なので、DirectXの左手系に直すために2->1->0の順にインデックスを格納していく
        indices[i * 3] = fbxMesh->GetPolygonVertex(i, 2);
        indices[i * 3 + 1] = fbxMesh->GetPolygonVertex(i, 1);
        indices[i * 3 + 2] = fbxMesh->GetPolygonVertex(i, 0);
    }
}

void FbxMeshParser::getNormalsAndUVs(
    Normals & normals,
    UVs & uvs,
    const Indices & indices,
    const FbxMesh * fbxMesh
) const {
    //頂点数
    int vertexCount = fbxMesh->GetControlPointsCount();

    //法線配列を取得する
    FbxArray<FbxVector4> normalArray;
    fbxMesh->GetPolygonVertexNormals(normalArray);
    //頂点数分の領域を確保する
    normals.reserve(vertexCount);

    //UV配列を取得する
    FbxArray<FbxVector2> uvArray;
    getUVArray(uvArray, fbxMesh);
    //頂点数分の領域を確保する
    uvs.reserve(vertexCount);

    std::unordered_map<int, std::vector<Vector3>> normalMap;
    for (int i = 0; i < normalArray.Size(); ++i) {
        //法線取得
        auto n = FbxUtility::fbxVector4ToVector3(normalArray[i]);
        n.x *= -1.f;
        normalMap[indices[i]].emplace_back(n);

        //UVを使用するなら
        if (uvArray.Size() > 0) {
            //UV取得
            const auto& uvArr = uvArray[i];
            auto& uv = uvs[indices[i]];
            uv.x = static_cast<float>(uvArr[0]);
            uv.y = 1.f - static_cast<float>(uvArr[1]);
        }
    }

    //多方向にまたがる法線を正規化する
    for (size_t i = 0; i < normalMap.size(); i++) {
        const auto& normalArr = normalMap[i];
        auto sumNormal = Vector3::zero;
        for (const auto& n : normalArr) {
            sumNormal += n;
        }
        normals[i] = Vector3::normalize(sumNormal / static_cast<float>(normalArr.size()));
    }
}

void FbxMeshParser::getUVArray(
    FbxArray<FbxVector2>&uvArray,
    const FbxMesh * fbxMesh
) const {
    FbxStringList uvNameList;

    //UVの名前リストを取得
    fbxMesh->GetUVSetNames(uvNameList);
    //UVリストの名前からUVを取得する
    fbxMesh->GetPolygonVertexUVs(uvNameList.GetStringAt(0), uvArray);
}
