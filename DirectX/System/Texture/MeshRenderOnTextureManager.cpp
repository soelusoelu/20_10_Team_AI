#include "MeshRenderOnTextureManager.h"
#include "../../Component/Engine/Sprite/MeshRenderOnTextureComponent.h"

MeshRenderOnTextureManager::MeshRenderOnTextureManager() {
    MeshRenderOnTextureComponent::setMeshRenderOnTextureManager(this);
}

MeshRenderOnTextureManager::~MeshRenderOnTextureManager() {
    MeshRenderOnTextureComponent::setMeshRenderOnTextureManager(nullptr);
}

void MeshRenderOnTextureManager::update() {
    removeDestroyTexture();
}

void MeshRenderOnTextureManager::add(const MeshRenderOnTexturePtr& texture) {
    mTextures.emplace_back(texture);
}

void MeshRenderOnTextureManager::drawMeshOnTextures(const Matrix4& viewProj) const {
    for (const auto& tex : mTextures) {
        tex->drawMeshOnTexture(viewProj);
    }
}

void MeshRenderOnTextureManager::drawTextures(const Matrix4& proj) const {
    for (const auto& tex : mTextures) {
        tex->draw(proj);
    }
}

void MeshRenderOnTextureManager::clear() {
    mTextures.clear();
}

void MeshRenderOnTextureManager::erase(const MeshRenderOnTexturePtr& texture) {
    mTextures.remove(texture);
}

void MeshRenderOnTextureManager::removeDestroyTexture() {
    auto itr = mTextures.begin();
    while (itr != mTextures.end()) {
        if ((*itr)->isDead()) {
            itr = mTextures.erase(itr);
        } else {
            ++itr;
        }
    }
}
