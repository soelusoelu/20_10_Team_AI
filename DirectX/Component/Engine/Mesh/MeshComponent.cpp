#include "MeshComponent.h"
#include "MeshRenderer.h"
#include "../Light/DirectionalLight.h"
#include "../../../GameObject/GameObject.h"
#include "../../../Imgui/imgui.h"
#include "../../../Mesh/Mesh.h"
#include "../../../System/AssetsManager.h"
#include "../../../Utility/LevelLoader.h"
#include "../../../Utility/FileUtil.h"

MeshComponent::MeshComponent(GameObject& gameObject)
    : Component(gameObject)
    , mMesh(nullptr)
    , mFileName()
    , mDirectoryPath()
    , mIsActive(true)
    , mShadowHandle(true)
    , mStarted(false)
{
}

MeshComponent::~MeshComponent() = default;

void MeshComponent::awake() {
    setActive(gameObject().getActive());
}

void MeshComponent::start() {
    mStarted = true;

    //メッシュが生成されてなければ終了
    if (!mMesh) {
        return;
    }

    //メッシュ描画コンポーネントをアタッチする
    attachMeshRenderer();
}

void MeshComponent::finalize() {
    destroy();
}

void MeshComponent::onEnable(bool value) {
    setActive(value);
}

void MeshComponent::loadProperties(const rapidjson::Value& inObj) {
    //ファイル名からメッシュを生成
    if (JsonHelper::getString(inObj, "fileName", &mFileName)) {
        if (!JsonHelper::getString(inObj, "directoryPath", &mDirectoryPath)) {
            mDirectoryPath = "Assets\\Model\\";
        }
        createMesh(mFileName, mDirectoryPath);
    }

    JsonHelper::getBool(inObj, "shadowHandle", &mShadowHandle);
}

void MeshComponent::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const {
    JsonHelper::setString(alloc, inObj, "fileName", mFileName);
    JsonHelper::setString(alloc, inObj, "directoryPath", mDirectoryPath);
}

void MeshComponent::drawInspector() {
    ImGui::Text("FileName: %s", (mDirectoryPath + mFileName).c_str());
}

void MeshComponent::createMeshFromFilePath(const std::string& filePath) {
    createMesh(FileUtil::getFileNameFromDirectry(filePath), FileUtil::getDirectryFromFilePath(filePath));
}

void MeshComponent::createMesh(const std::string& fileName, const std::string& directoryPath) {
    mMesh = AssetsManager::instance().createMesh(fileName, directoryPath);
    mFileName = fileName;
    mDirectoryPath = directoryPath;

    //すでにstart関数が呼ばれていたらメッシュ描画コンポーネントをアタッチする
    if (mStarted) {
        attachMeshRenderer();
    }
}

bool MeshComponent::isDraw() const {
    if (!mMesh) {
        return false;
    }
    if (!getActive()) {
        return false;
    }

    return true;
}

void MeshComponent::setActive(bool value) {
    mIsActive = value;
}

bool MeshComponent::getActive() const {
    return mIsActive;
}

IMesh* MeshComponent::getMesh() const {
    return (mMesh) ? mMesh.get() : nullptr;
}

IAnimation* MeshComponent::getAnimation() const {
    return (mMesh) ? mMesh.get() : nullptr;
}

const IMeshDrawer* MeshComponent::getDrawer() const {
    return (mMesh) ? mMesh.get() : nullptr;
}

bool MeshComponent::handleShadow() const {
    return mShadowHandle;
}

void MeshComponent::attachMeshRenderer() {
    const auto& meshRenderer = getComponent<MeshRenderer>();
    if (!meshRenderer) {
        addComponent<MeshRenderer>("MeshRenderer");
    }
}
