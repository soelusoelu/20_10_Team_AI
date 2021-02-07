#include "MeshRenderOnTextureComponent.h"
#include "../../../System/Texture/MeshRenderOnTexture.h"
#include "../../../System/Texture/MeshRenderOnTextureManager.h"
#include "../../../Engine/DebugManager/DebugUtility/Debug.h"
#include "../../../Utility/LevelLoader.h"

MeshRenderOnTextureComponent::MeshRenderOnTextureComponent(GameObject& gameObject)
    : Component(gameObject)
    , mMeshRenderOnTexture(nullptr)
{
}

MeshRenderOnTextureComponent::~MeshRenderOnTextureComponent() = default;

void MeshRenderOnTextureComponent::start() {
    if (!mManager) {
        Debug::logError("MeshRenderOnTextureManager is null.");
        return;
    }

    mManager->add(shared_from_this());
}

void MeshRenderOnTextureComponent::finalize() {
    destroy();
}

void MeshRenderOnTextureComponent::loadProperties(const rapidjson::Value& inObj) {
    if (std::string path; JsonHelper::getString(inObj, "filePath", &path)) {
        int width = DEFAULT_SPRITE_WIDTH;
        int height = DEFAULT_SPRITE_HEIGHT;
        JsonHelper::getInt(inObj, "width", &width);
        JsonHelper::getInt(inObj, "height", &height);

        mMeshRenderOnTexture = std::make_unique<MeshRenderOnTexture>(path, width, height);
    }
}

void MeshRenderOnTextureComponent::drawMeshOnTexture(const Matrix4& viewProj) const {
    if (!mMeshRenderOnTexture) {
        return;
    }
    mMeshRenderOnTexture->drawMeshOnTexture(viewProj);
}

void MeshRenderOnTextureComponent::draw(const Matrix4& proj) const {
    if (!mMeshRenderOnTexture) {
        return;
    }
    mMeshRenderOnTexture->draw(proj);
}

void MeshRenderOnTextureComponent::changeMesh(const std::string& filePath) {
    mMeshRenderOnTexture->changeMesh(filePath);
}

void MeshRenderOnTextureComponent::setPositionForTexture(const Vector2& pos) {
    mMeshRenderOnTexture->setPositionForTexture(pos);
}

void MeshRenderOnTextureComponent::setSizeForTexture(const Vector2& size) {
    mMeshRenderOnTexture->setSizeForTexture(size);
}

const IMesh& MeshRenderOnTextureComponent::getMesh() const {
    return mMeshRenderOnTexture->getMesh();
}

const Sprite& MeshRenderOnTextureComponent::getSprite() const {
    return mMeshRenderOnTexture->getSprite();
}

const std::string& MeshRenderOnTextureComponent::getFilePath() const {
    return mMeshRenderOnTexture->getFilePath();
}

void MeshRenderOnTextureComponent::setMeshRenderOnTextureManager(MeshRenderOnTextureManager* manager) {
    mManager = manager;
}
