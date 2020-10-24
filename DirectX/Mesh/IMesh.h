#pragma once

#include "IMeshLoader.h"
#include "Material.h"
#include <vector>

//外部公開用メッシュインターフェース
class IMesh {
public:
    virtual ~IMesh() = default;
    //マテリアルの取得
    virtual const Material& getMaterial(unsigned index) const = 0;
    //メッシュの数を取得する
    virtual unsigned getMeshCount() const = 0;
    //すべての頂点情報を取得
    virtual const std::vector<MeshVertices>& getMeshesVertices() const = 0;
};
