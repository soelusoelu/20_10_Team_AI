#include "MeshManager.h"
#include "../Component/Camera/Camera.h"
#include "../Component/Mesh/MeshComponent.h"
#include "../Component/Mesh/ShadowMap.h"
#include "../DirectX/DirectXInclude.h"
#include "../Transform/Transform3D.h"

MeshManager::MeshManager() {
    MeshComponent::setMeshManager(this);
}

MeshManager::~MeshManager() {
    MeshComponent::setMeshManager(nullptr);
}

void MeshManager::update() {
    remove();
}

void MeshManager::draw(const Camera& camera, const DirectionalLight& dirLight) const {
    if (mMeshes.empty()) {
        return;
    }

    MyDirectX::DirectX::instance().rasterizerState()->setCulling(CullMode::BACK);

    if (mShadowMap) {
        for (const auto& mesh : mMeshes) {
            if (!isDraw(*mesh, camera)) {
                continue;
            }

            mShadowMap->draw(camera, dirLight);
        }
    }

    for (const auto& mesh : mMeshes) {
        if (!isDraw(*mesh, camera)) {
            continue;
        }

        mesh->draw(camera, dirLight);
    }
}

void MeshManager::add(const MeshPtr& mesh) {
    mMeshes.emplace_back(mesh);
}

void MeshManager::addShadowMap(const std::shared_ptr<ShadowMap>& shadowMap) {
    mShadowMap = shadowMap;
}

void MeshManager::clear() {
    remove();
}

void MeshManager::remove() {
    auto itr = mMeshes.begin();
    while (itr != mMeshes.end()) {
        if ((*itr)->isDead()) {
            itr = mMeshes.erase(itr);
        } else {
            ++itr;
        }
    }
}

bool MeshManager::isDraw(const MeshComponent& mesh, const Camera& camera) const {
    if (!mesh.getActive()) {
        return false;
    }
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
