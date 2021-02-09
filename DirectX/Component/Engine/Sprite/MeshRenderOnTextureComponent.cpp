#include "MeshRenderOnTextureComponent.h"
#include "../Camera/CameraHelper.h"
#include "../../../Collision/Collision.h"
#include "../../../Sprite/Sprite.h"
#include "../../../System/Texture/MeshRenderOnTexture.h"
#include "../../../System/Texture/MeshRenderOnTextureManager.h"
#include "../../../Engine/DebugManager/DebugUtility/Debug.h"
#include "../../../Utility/LevelLoader.h"

MeshRenderOnTextureComponent::MeshRenderOnTextureComponent(GameObject& gameObject)
    : Component(gameObject)
    , mMeshRenderOnTexture(nullptr)
    , mSphereCoverMesh()
    , mView()
    , mProjection()
{
}

MeshRenderOnTextureComponent::~MeshRenderOnTextureComponent() = default;

void MeshRenderOnTextureComponent::start() {
    if (!mManager) {
        Debug::logError("MeshRenderOnTextureManager is null.");
        return;
    }

    //射影行列を作成する
    mProjection = Matrix4::createPerspectiveFOV(1.f, 45.f, 0.1f, 1000.f);

    mManager->add(shared_from_this());
}

void MeshRenderOnTextureComponent::update() {
    mMeshRenderOnTexture->getSprite().computeWorldTransform();
}

void MeshRenderOnTextureComponent::finalize() {
    destroy();
}

void MeshRenderOnTextureComponent::onEnable(bool value) {
    mMeshRenderOnTexture->getSprite().setActive(value);
}

void MeshRenderOnTextureComponent::loadProperties(const rapidjson::Value& inObj) {
    if (std::string path; JsonHelper::getString(inObj, "filePath", &path)) {
        int width = DEFAULT_SPRITE_WIDTH;
        int height = DEFAULT_SPRITE_HEIGHT;
        JsonHelper::getInt(inObj, "width", &width);
        JsonHelper::getInt(inObj, "height", &height);

        mMeshRenderOnTexture = std::make_unique<MeshRenderOnTexture>(path, width, height);
        calcView();
    }
}

void MeshRenderOnTextureComponent::drawMeshOnTexture() const {
    if (!mMeshRenderOnTexture) {
        return;
    }
    mMeshRenderOnTexture->drawMeshOnTexture(mView * mProjection);
}

void MeshRenderOnTextureComponent::draw(const Matrix4& proj) const {
    if (!mMeshRenderOnTexture) {
        return;
    }
    mMeshRenderOnTexture->draw(proj);
}

void MeshRenderOnTextureComponent::changeMeshFromFilePath(const std::string& filePath) {
    if (!mMeshRenderOnTexture) {
        mMeshRenderOnTexture = std::make_unique<MeshRenderOnTexture>(filePath, DEFAULT_SPRITE_WIDTH, DEFAULT_SPRITE_HEIGHT);
    } else {
        mMeshRenderOnTexture->changeMesh(filePath);
    }

    calcView();
}

void MeshRenderOnTextureComponent::changeMesh(const std::string& fileName, const std::string& directoryPath) {
    changeMeshFromFilePath(directoryPath + fileName);
}

void MeshRenderOnTextureComponent::setPositionForTexture(const Vector2& pos) {
    mMeshRenderOnTexture->setPositionForTexture(pos);
}

void MeshRenderOnTextureComponent::setSizeForTexture(const Vector2& size) {
    mMeshRenderOnTexture->setSizeForTexture(size);

}

Sprite& MeshRenderOnTextureComponent::getSprite() const {
    return mMeshRenderOnTexture->getSprite();
}

const IMesh& MeshRenderOnTextureComponent::getMesh() const {
    return mMeshRenderOnTexture->getMesh();
}

const std::string& MeshRenderOnTextureComponent::getFilePath() const {
    return mMeshRenderOnTexture->getFilePath();
}

int MeshRenderOnTextureComponent::getWidth() const {
    return mMeshRenderOnTexture->getWidth();
}

int MeshRenderOnTextureComponent::getHeight() const {
    return mMeshRenderOnTexture->getHeight();
}

void MeshRenderOnTextureComponent::setMeshRenderOnTextureManager(MeshRenderOnTextureManager* manager) {
    mManager = manager;
}

void MeshRenderOnTextureComponent::calcView() {
    SphereHelper::create(mSphereCoverMesh, getMesh());
    mView = CameraHelper::getViewMatrixTakingSphereInCamera(mSphereCoverMesh, getWidth(), getHeight(), 45.f, Vector3::forward, Vector3::up);
}
