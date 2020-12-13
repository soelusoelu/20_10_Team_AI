#include "MeshManager.h"
#include "../Component/Camera/Camera.h"
#include "../Component/Mesh/MeshComponent.h"
#include "../Component/Mesh/MeshRenderer.h"
#include "../Component/Mesh/ShadowMap.h"
#include "../DirectX/DirectXInclude.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectFactory.h"
#include "../Transform/Transform3D.h"

MeshManager::MeshManager()
    : mShadowMap(nullptr)
{
    MeshRenderer::setMeshManager(this);
}

MeshManager::~MeshManager() {
    MeshRenderer::setMeshManager(nullptr);
}

void MeshManager::createShadowMap() {
    auto sm = GameObjectCreater::create("ShadowMap");
    mShadowMap = sm->componentManager().getComponent<ShadowMap>();
}

void MeshManager::update() {
    remove();
}

void MeshManager::draw(const Camera& camera, const DirectionalLight& dirLight) const {
    if (mShadowMeshes.empty()) {
        return;
    }

    MyDirectX::DirectX::instance().rasterizerState()->setCulling(CullMode::BACK);

    if (mShadowMap) {
        drawShadow(camera, dirLight);
    }

    drawMeshes(camera, dirLight);
}

void MeshManager::add(const MeshPtr& mesh, bool handleShadow) {
    if (handleShadow) {
        mShadowMeshes.emplace_back(mesh);
    } else {
        mMeshes.emplace_back(mesh);
    }
}

void MeshManager::clear() {
    remove();
}

void MeshManager::remove() {
    auto itr = mShadowMeshes.begin();
    while (itr != mShadowMeshes.end()) {
        if ((*itr)->getMeshComponent().isDead()) {
            itr = mShadowMeshes.erase(itr);
        } else {
            ++itr;
        }
    }

    auto itr2 = mMeshes.begin();
    while (itr2 != mMeshes.end()) {
        if ((*itr2)->getMeshComponent().isDead()) {
            itr2 = mMeshes.erase(itr2);
        } else {
            ++itr2;
        }
    }
}

bool MeshManager::isDraw(const MeshRenderer& mesh, const Camera& camera) const {
    if (mesh.isDead()) {
        return false;
    }
    //auto pos = mesh.transform().getPosition();
    //auto radius = mesh.getMesh().getRadius() * mesh.transform().getScale().x;
    //if (!camera.viewFrustumCulling(pos, radius)) {
    //    return false;
    //}

    return true;
}

void MeshManager::drawMeshes(const Camera& camera, const DirectionalLight& dirLight) const {
    //影の影響を受けるメッシュの描画
    for (const auto& mesh : mShadowMeshes) {
        if (!isDraw(*mesh, camera)) {
            continue;
        }

        //深度テクスチャを転送する
        mShadowMap->transferShadowTexture();

        mesh->draw(camera, dirLight);

        //深度テクスチャの後処理
        mShadowMap->drawEndShadowTexture();
    }

    //影の影響を受けないメッシュの描画
    for (const auto& mesh : mMeshes) {
        if (!isDraw(*mesh, camera)) {
            continue;
        }

        mesh->draw(camera, dirLight);
    }
}

void MeshManager::drawShadow(const Camera& camera, const DirectionalLight& dirLight) const {
    //描画準備
    mShadowMap->drawBegin(camera, dirLight);

    for (const auto& mesh : mShadowMeshes) {
        //描画できないなら次へ
        if (!isDraw(*mesh, camera)) {
            continue;
        }

        //描画
        mShadowMap->draw(*mesh);

        //影描画用の定数バッファを設定する
        mShadowMap->setShadowConstantBuffer(*mesh);
    }

    //描画終了処理
    mShadowMap->drawEnd();
}
