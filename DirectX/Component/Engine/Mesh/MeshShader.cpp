#include "MeshShader.h"
#include "MeshMaterial.h"
#include "../Mesh/MeshComponent.h"
#include "../../../Engine/DebugManager/DebugUtility/Debug.h"
#include "../../../Imgui/imgui.h"
#include "../../../Mesh/Material.h"
#include "../../../Mesh/MeshCommonShaderSetter.h"
#include "../../../System/AssetsManager.h"
#include "../../../System/Shader/ConstantBuffers.h"
#include "../../../System/Shader/Shader.h"
#include "../../../System/Texture/Texture.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/LevelLoader.h"

MeshShader::MeshShader(GameObject& gameObject)
    : Component(gameObject)
    , mMeshMaterial(nullptr)
    , mMesh(nullptr)
    , mAnimation(nullptr)
    , mShader(nullptr)
{
}

MeshShader::~MeshShader() = default;

void MeshShader::start() {
    mMeshMaterial = getComponent<MeshMaterial>();
    if (!mMeshMaterial) {
        mMeshMaterial = addComponent<MeshMaterial>("MeshMaterial");
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

void MeshShader::drawInspector() {
    ImGui::Text("Shader: %s", mShader->getShaderName().c_str());
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

void MeshShader::setCommonValue(
    const Matrix4& view,
    const Matrix4& projection,
    const Vector3& cameraPosition,
    const Vector3& dirLightDirection,
    const Vector3& dirLightColor
) const {
    //シェーダーのコンスタントバッファーに各種データを渡す
    MeshCommonConstantBuffer meshcb{};
    MeshCommonShaderSetter::setCommon(meshcb, transform().getWorldTransform(), view, projection, cameraPosition, dirLightDirection, dirLightColor);
    mShader->transferData(&meshcb, sizeof(meshcb), 0);
}

void MeshShader::setDefaultMaterial(unsigned materialIndex, unsigned constantBufferIndex) const {
    if (mMeshMaterial->isSetMaterial(materialIndex)) {
        //指定のインデックスのマテリアルが登録されてたらそっちを使う
        setMaterial(mMeshMaterial->getMaterial(materialIndex), constantBufferIndex);
    } else {
        //登録されてなかったら、デフォルトを使う
        setMaterial(mMesh->getMaterial(materialIndex), constantBufferIndex);
    }
}

void MeshShader::setTransferData(const void* data, unsigned size, unsigned constantBufferIndex) {
    mTransferDataMap[constantBufferIndex] = { data, size };
}

void MeshShader::setInterface(const IMesh* mesh, const IAnimation* anim) {
    mMesh = mesh;
    mAnimation = anim;
    setDefaultShader();
}

void MeshShader::setDefaultShader() {
    std::string shader;
    //影の影響を受けるか
    if (getComponent<MeshComponent>()->handleShadow()) {
        shader = "Shadow.hlsl";
        //ボーンが有るなら
        if (mAnimation->getBoneCount() > 0) {
            shader = "SkinMeshShadow.hlsl";
        }
    } else {
        shader = "Mesh.hlsl";
        //ボーンが有るなら
        if (mAnimation->getBoneCount() > 0) {
            shader = "SkinMesh.hlsl";
        }
    }

    //シェーダーを生成する
    mShader = AssetsManager::instance().createShader(shader);
}

void MeshShader::setMaterial(const Material& material, unsigned constantBufferIndex) const {
    MaterialConstantBuffer matcb{};
    MeshCommonShaderSetter::setMaterial(matcb, material);
    mShader->transferData(&matcb, sizeof(matcb), constantBufferIndex);
}
