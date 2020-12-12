﻿#include "MeshComponent.h"
#include "MeshRenderer.h"
#include "../Light/DirectionalLight.h"
#include "../../GameObject/GameObject.h"
#include "../../Imgui/imgui.h"
#include "../../Mesh/Mesh.h"
#include "../../System/AssetsManager.h"
#include "../../Utility/LevelLoader.h"

MeshComponent::MeshComponent(GameObject& gameObject)
    : Component(gameObject)
    , mMesh(nullptr)
    , mFileName()
    , mDirectoryPath()
    , mState(State::ACTIVE)
    , mShadowHandle(true)
{
}

MeshComponent::~MeshComponent() = default;

void MeshComponent::awake() {
    setActive(gameObject().getActive());
}

void MeshComponent::start() {
    auto meshRenderer = getComponent<MeshRenderer>();
    if (!meshRenderer) {
        addComponent<MeshRenderer>("MeshRenderer");
    }
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

void MeshComponent::createMesh(const std::string& fileName, const std::string& directoryPath) {
    mMesh = AssetsManager::instance().createMesh(fileName, directoryPath);
    mFileName = fileName;
    mDirectoryPath = directoryPath;
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

void MeshComponent::destroy() {
    mState = State::DEAD;
}

void MeshComponent::setActive(bool value) {
    mState = (value) ? State::ACTIVE : State::NON_ACTIVE;
}

bool MeshComponent::getActive() const {
    return mState == State::ACTIVE;
}

bool MeshComponent::isDead() const {
    return mState == State::DEAD;
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
