﻿#include "MeshOutLine.h"
#include "SkinMeshComponent.h"
#include "../Camera/Camera.h"
#include "../Light/DirectionalLight.h"
#include "../../DebugLayer/Debug.h"
#include "../../DebugLayer/ImGuiWrapper.h"
#include "../../DirectX/DirectXInclude.h"
#include "../../Imgui/imgui.h"
#include "../../Mesh/Material.h"
#include "../../Mesh/Mesh.h"
#include "../../System/AssetsManager.h"
#include "../../System/Shader/ConstantBuffers.h"
#include "../../System/Shader/Shader.h"
#include "../../System/Texture/TextureFromFile.h"
#include "../../Transform/Transform3D.h"
#include "../../Utility/LevelLoader.h"

MeshOutLine::MeshOutLine(GameObject& gameObject)
    : MeshComponent(gameObject)
    , mOutLineShader(nullptr)
    , mOutLineColor(ColorPalette::white)
    , mOutLineThickness(0.1f)
    , mIsDrawOutLine(true)
    , mIsAnimation(false)
    , mOffset(0.f)
{
}

MeshOutLine::~MeshOutLine() = default;

void MeshOutLine::awake() {
    //ボーンの有無でアニメーションするモデルか判断する
    mIsAnimation = (mMesh->getBoneCount() > 0);

    //アニメーションするかでシェーダーを決める
    auto name = (mIsAnimation) ? "SkinMeshOutLine.hlsl" : "OutLine.hlsl";
    mOutLineShader = AssetsManager::instance().createShader(name);

    MeshComponent::awake();
}

void MeshOutLine::start() {
    //スキンメッシュコンポーネントのアタッチがメッシュコンポーネントのstartで行うから先
    MeshComponent::start();

    if (mIsAnimation) {
        mSkinMesh = getComponent<SkinMeshComponent>();
    }
}

void MeshOutLine::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getVector3(inObj, "outLineColor", &mOutLineColor);
    JsonHelper::getFloat(inObj, "outLineColorThickness", &mOutLineThickness);
    JsonHelper::getBool(inObj, "isDrawOutLine", &mIsDrawOutLine);
    JsonHelper::getFloat(inObj, "offset", &mOffset);

    MeshComponent::loadProperties(inObj);
}

void MeshOutLine::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const {
    JsonHelper::setVector3(alloc, inObj, "outLineColor", mOutLineColor);
    JsonHelper::setFloat(alloc, inObj, "outLineColorThickness", mOutLineThickness);
    JsonHelper::setBool(alloc, inObj, "isDrawOutLine", mIsDrawOutLine);
    JsonHelper::setFloat(alloc, inObj, "offset", mOffset);

    MeshComponent::saveProperties(alloc, inObj);
}

void MeshOutLine::drawInspector() {
    ImGuiWrapper::colorEdit3("OutLineColor", mOutLineColor);
    ImGui::SliderFloat("OutLineThickness", &mOutLineThickness, 0.f, 1.f);
    ImGui::Checkbox("IsDrawOutLine", &mIsDrawOutLine);
    ImGuiWrapper::dragFloat("Offset", mOffset, 0.01f);

    MeshComponent::drawInspector();
}

void MeshOutLine::draw(const Camera& camera, const DirectionalLight& dirLight) const {
    if (mIsDrawOutLine) {
        //裏面のみ描画したいから
        MyDirectX::DirectX::instance().rasterizerState()->setCulling(CullMode::FRONT);

        //アウトライン描画
        drawOutLine(camera, dirLight);

        //設定を戻す
        MyDirectX::DirectX::instance().rasterizerState()->setCulling(CullMode::BACK);
    }

    //メッシュの描画は上層クラスに任せる
    MeshComponent::draw(camera, dirLight);
}

void MeshOutLine::setOutLineColor(const Vector3& color) {
    mOutLineColor = color;
}

const Vector3& MeshOutLine::getOutLineColor() const {
    return mOutLineColor;
}

void MeshOutLine::setOutLineThickness(float thickness) {
    mOutLineThickness = thickness;
}

float MeshOutLine::getOutLineThickness() const {
    return mOutLineThickness;
}

void MeshOutLine::setActiveOutLine(bool value) {
    mIsDrawOutLine = value;
}

bool MeshOutLine::getActiveOutLine() const {
    return mIsDrawOutLine;
}

void MeshOutLine::drawOutLine(const Camera& camera, const DirectionalLight& dirLight) const {
    //アウトラインシェーダーの登録
    mOutLineShader->setShaderInfo();

    //スケールを拡大したモデルをアウトラインとして描画するため
    //ワールド行列の再計算をする
    const auto& t = transform();
    auto offset = Vector3::up * mOffset;
    auto world = Matrix4::createTranslation(-t.getPivot() - offset);
    world *= Matrix4::createScale(t.getScale() + Vector3::one * mOutLineThickness);
    world *= Matrix4::createFromQuaternion(t.getRotation());
    world *= Matrix4::createTranslation(t.getPosition() + offset * t.getScale());

    //シェーダーのコンスタントバッファーに各種データを渡す
    MeshCommonConstantBuffer meshcb;
    meshcb.world = world;
    meshcb.view = camera.getView();
    meshcb.projection = camera.getProjection();
    meshcb.wvp = world * camera.getViewProjection();
    meshcb.lightDir = dirLight.getDirection();
    meshcb.lightColor = dirLight.getLightColor();
    meshcb.cameraPos = camera.getPosition();
    mOutLineShader->transferData(&meshcb, sizeof(meshcb), 0);

    OutLineConstantBuffer outlinecb;
    outlinecb.outlineColor = Vector4(mOutLineColor, 1.f);
    mOutLineShader->transferData(&outlinecb, sizeof(outlinecb), 1);

    //アニメーションするならボーンのデータも渡す
    if (mIsAnimation) {
        std::shared_ptr<SkinMeshComponent> sm = nullptr;
        sm = mSkinMesh.lock();
        if (!sm) {
            Debug::logError("Failed lock. SkinMeshComponent is null.");
            return;
        }

        //ボーンデータを転送する
        mOutLineShader->transferData(sm->getBoneCurrentFrameMatrix().data(), sizeof(SkinMeshConstantBuffer), 2);
    }

    //アウトラインを描画する
    for (size_t i = 0; i < mMesh->getMeshCount(); ++i) {
        mMesh->draw(i);
    }
}