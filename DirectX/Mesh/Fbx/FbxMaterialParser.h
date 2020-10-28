#pragma once

#include "../Material.h"
#include <fbxsdk.h>
#include <string>

//FBXマテリアル解析クラス
class FbxMaterialParser {
public:
    FbxMaterialParser();
    ~FbxMaterialParser();

    //マテリアルを解析する
    void parse(
        Material& material,
        FbxMesh* fbxMesh,
        const std::string& directoryPath
    );

private:
    //マテリアルの各属性を読み込む
    void loadMaterialAttribute(
        Material& material,
        FbxSurfaceMaterial* fbxSurfaceMaterial
    );

    //Lambert型のマテリアルを読み込む
    void loadLambert(
        Material& material,
        const FbxSurfaceLambert* fbxSurfaceLambert
    );

    //Phong型のマテリアルを読み込む
    void loadPhong(
        Material& material,
        const FbxSurfacePhong* fbxSurfacePhong
    );

    //テクスチャ読み込み
    void loadTextures(
        Material& material,
        FbxSurfaceMaterial* fbxSurfaceMaterial,
        const std::string& directoryPath
    );

    //テクスチャ作成
    void createTexture(
        Material& material,
        const FbxSurfaceMaterial* fbxSurfaceMaterial,
        const std::string& directoryPath,
        const char* type
    );

    //プロパティからFBXのテクスチャを取得する
    FbxFileTexture* getFbxTexture(const FbxProperty& prop) const;
};
