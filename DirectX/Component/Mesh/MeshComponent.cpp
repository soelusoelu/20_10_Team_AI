#include "MeshComponent.h"
#include "MeshShader.h"
#include "SkinMeshComponent.h"
#include "../Camera/Camera.h"
#include "../Light/DirectionalLight.h"
#include "../../DebugLayer/Debug.h"
#include "../../GameObject/GameObject.h"
#include "../../Imgui/imgui.h"
#include "../../Mesh/Mesh.h"
#include "../../Mesh/MeshManager.h"
#include "../../System/AssetsManager.h"
#include "../../System/Shader/ConstantBuffers.h"
#include "../../System/Shader/Shader.h"
#include "../../System/Texture/Texture.h"
#include "../../Transform/Transform3D.h"
#include "../../Utility/LevelLoader.h"

MeshComponent::MeshComponent(GameObject& gameObject)
    : Component(gameObject)
    , mMesh(nullptr)
    , mMeshShader(nullptr)
    , mFileName()
    , mDirectoryPath()
    , mState(State::ACTIVE)
    , mAlpha(1.f)
{
}

MeshComponent::~MeshComponent() = default;

void MeshComponent::awake() {
    setActive(gameObject().getActive());
}

void MeshComponent::start() {
    mMeshShader = getComponent<MeshShader>();
    //MeshShaderがないなら追加する
    if (!mMeshShader) {
        mMeshShader = addComponent<MeshShader>("MeshShader");
        mMeshShader->setMeshComponent(shared_from_this());
    }

    //ボーンが有るモデルなら
    if (mMesh->getBoneCount() > 0) {
        auto skinMesh = getComponent<SkinMeshComponent>();
        //SkinMeshComponentがアタッチされてないなら追加する
        if (!skinMesh) {
            skinMesh = addComponent<SkinMeshComponent>("SkinMeshComponent");
            skinMesh->setMeshAndShader(shared_from_this(), mMeshShader);
        }
    }

    if (mMesh) {
        addToManager();
    }
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

    //アルファ値を取得する
    JsonHelper::getFloat(inObj, "alpha", &mAlpha);
}

void MeshComponent::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const {
    JsonHelper::setString(alloc, inObj, "fileName", mFileName);
    JsonHelper::setString(alloc, inObj, "directoryPath", mDirectoryPath);
    JsonHelper::setFloat(alloc, inObj, "alpha", mAlpha);
}

void MeshComponent::drawInspector() {
    ImGui::Text("FileName: %s", (mDirectoryPath + mFileName).c_str());
    ImGui::SliderFloat("Alpha", &mAlpha, 0.f, 1.f);
}

void MeshComponent::draw(const Camera& camera, const DirectionalLight& dirLight) const {
    //描画前準備
    mMeshShader->preSet();

    //メッシュ共通の値を設定する
    mMeshShader->setCommonValue(camera, dirLight);

    //コンスタントバッファに転送する
    mMeshShader->transferData();

    for (size_t i = 0; i < mMesh->getMeshCount(); ++i) {
        //マテリアルを設定する
        mMeshShader->setDefaultMaterial(i);

        //描画
        mMesh->draw(i);
    }
}

void MeshComponent::createMesh(const std::string& fileName, const std::string& directoryPath) {
    mMesh = AssetsManager::instance().createMesh(fileName, directoryPath);
    mFileName = fileName;
    mDirectoryPath = directoryPath;
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

const IMesh& MeshComponent::getMesh() const {
    return *mMesh;
}

void MeshComponent::setAlpha(float alpha) {
    mAlpha = alpha;
}

float MeshComponent::getAlpha() const {
    return mAlpha;
}

void MeshComponent::setMeshManager(MeshManager* manager) {
    mMeshManager = manager;
}

void MeshComponent::addToManager() {
    //マネージャーが登録されていなかったら終了する
    if (!mMeshManager) {
        Debug::logWarning("The mesh manager is not registered.");
        return;
    }

    //マネージャーに自身を登録する
    mMeshManager->add(shared_from_this());
}
