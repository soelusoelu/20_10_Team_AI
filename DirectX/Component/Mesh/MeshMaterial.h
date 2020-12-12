#pragma once

#include "../Component.h"
#include "../../Mesh/Material.h"
#include <memory>
#include <unordered_map>

//マテリアル管理クラス
class MeshMaterial : public Component {
    using MaterialPtr = std::shared_ptr<Material>;
    using MaterialMap = std::unordered_map<unsigned, MaterialPtr>;

public:
    MeshMaterial(GameObject& gameObject);
    ~MeshMaterial();
    //マテリアルを設定する
    void setMaterial(const MaterialPtr& material, unsigned index);
    //マテリアルを取得する
    const Material& getMaterial(unsigned index);
    //マテリアルが設定されているか
    bool isSetMaterial(unsigned index) const;

private:
    MeshMaterial(const MeshMaterial&) = delete;
    MeshMaterial& operator=(const MeshMaterial&) = delete;

private:
    MaterialMap mMaterials;
};
