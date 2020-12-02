﻿#include "MeshShader.h"
#include "../Camera/Camera.h"
#include "../Light/DirectionalLight.h"
#include "../Mesh/MeshComponent.h"
#include "../../DebugLayer/Debug.h"
#include "../../Mesh/IMesh.h"
#include "../../Mesh/Material.h"
#include "../../System/AssetsManager.h"
#include "../../System/Shader/ConstantBuffers.h"
#include "../../System/Shader/Shader.h"
#include "../../System/Texture/Texture.h"
#include "../../Transform/Transform3D.h"
#include "../../Utility/LevelLoader.h"

MeshShader::MeshShader(GameObject& gameObject)
    : Component(gameObject)
    , mShader(nullptr)
{
}

MeshShader::~MeshShader() = default;

void MeshShader::start() {
    auto mesh = mMesh.lock();
    if (!mesh) {
        mMesh = getComponent<MeshComponent>();
    }
}

void MeshShader::loadProperties(const rapidjson::Value& inObj) {
    std::string shader;
    //シェーダー名が取得できたら読み込む
    if (JsonHelper::getString(inObj, "shaderName", &shader)) {
        //シェーダーを生成する
        mShader = AssetsManager::instance().createShader(shader);
    } else {
        //できなかったらデフォルトを使う
        setDefaultShader();
    }
}

void MeshShader::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const {
    JsonHelper::setString(alloc, inObj, "shaderName", mShader->getShaderName());
}

void MeshShader::preSet() const {
    //使用するシェーダーの登録
    mShader->setShaderInfo();
}

void MeshShader::transferData() {
    //何も登録されてないなら終了
    if (mTransferDataMap.empty()) {
        return;
    }

    //すべてのデータを転送する
    for (const auto& transferData : mTransferDataMap) {
        const auto& data = transferData.second;
        mShader->transferData(data.data, data.size, transferData.first);
    }
}

void MeshShader::setCommonValue(const Camera& camera, const DirectionalLight& dirLight) const {
    //シェーダーのコンスタントバッファーに各種データを渡す
    MeshCommonConstantBuffer meshcb;
    const auto& world = transform().getWorldTransform();
    meshcb.world = world;
    meshcb.view = camera.getView();
    meshcb.projection = camera.getProjection();
    meshcb.wvp = world * camera.getViewProjection();
    meshcb.lightDir = dirLight.getDirection();
    meshcb.lightColor = dirLight.getLightColor();
    meshcb.cameraPos = camera.getPosition();
    mShader->transferData(&meshcb, sizeof(meshcb), 0);
}

void MeshShader::setDefaultMaterial(unsigned materialIndex, unsigned constantBufferIndex) const {
    auto mesh = getMeshComponent();
    if (!mesh) {
        return;
    }

    const auto& iMesh = mesh->getMesh();

    MaterialConstantBuffer matcb;
    const auto& mat = iMesh.getMaterial(materialIndex);
    matcb.ambient = mat.ambient;
    float alpha = mesh->getAlpha();
    //アルファ値は0のときが多いから
    if (!Math::nearZero(mat.transparency)) {
        alpha *= mat.transparency;
    }
    matcb.diffuse = Vector4(mat.diffuse * mesh->getColorRatio(), alpha);
    matcb.specular = mat.specular;
    matcb.shininess = mat.shininess;
    //データ転送
    mShader->transferData(&matcb, sizeof(matcb), constantBufferIndex);

    //テクスチャ登録
    mat.texture->setTextureInfo();
}

void MeshShader::setTransferData(const void* data, unsigned size, unsigned constantBufferIndex) {
    TransferData temp{ data, size };
    if (mTransferDataMap.find(constantBufferIndex) == mTransferDataMap.end()) {
        //未登録なら新規に登録する
        mTransferDataMap.emplace(constantBufferIndex, temp);
    } else {
        //登録済みなら更新する
        mTransferDataMap[constantBufferIndex] = temp;
    }
}

void MeshShader::setMeshComponent(const std::shared_ptr<MeshComponent>& mesh) {
    mMesh = mesh;
    setDefaultShader();
}

void MeshShader::setDefaultShader() {
    auto mesh = getMeshComponent();
    if (!mesh) {
        return;
    }

    const auto& iMesh = mesh->getMesh();

    std::string shader = "Mesh.hlsl";
    //ノーマルマップが有るなら
    if (iMesh.getMaterial(0).normalMapTexture) {
        shader = "NormalMap.hlsl";
    }
    //ボーンが有るなら
    if (iMesh.getBoneCount() > 0) {
        shader = "SkinMesh.hlsl";
    }
    //シェーダーを生成する
    mShader = AssetsManager::instance().createShader(shader);
}

std::shared_ptr<MeshComponent> MeshShader::getMeshComponent() const {
    std::shared_ptr<MeshComponent> mesh = nullptr;
    mesh = mMesh.lock();
    //ロックに失敗したらエラーログ
    if (!mesh) {
        Debug::logError("Failed lock. MeshComponent is null.");
    }

    return mesh;
}