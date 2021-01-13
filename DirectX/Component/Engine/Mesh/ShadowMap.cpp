﻿#include "ShadowMap.h"
#include "MeshComponent.h"
#include "MeshRenderer.h"
#include "MeshShader.h"
#include "../Sprite/SpriteComponent.h"
#include "../../../DirectX/DirectXInclude.h"
#include "../../../Imgui/imgui.h"
#include "../../../Math/Math.h"
#include "../../../System/AssetsManager.h"
#include "../../../System/Window.h"
#include "../../../System/Shader/Shader.h"
#include "../../../System/Texture/Texture.h"
#include "../../../System/Texture/RenderTexture.h"
#include "../../../Transform/Transform2D.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/LevelLoader.h"

ShadowMap::ShadowMap()
    : mDepthTextureCreateShader(nullptr)
    , mRenderTexture(nullptr)
    , mLightDistance(0.f)
    , mNearClip(0.f)
    , mFarClip(0.f)
{
}

ShadowMap::~ShadowMap() = default;

void ShadowMap::initialize() {
    mDepthTextureCreateShader = AssetsManager::instance().createShader("ShadowDepthTextureCreater.hlsl");
    mRenderTexture = std::make_unique<RenderTexture>(Window::width(), Window::height(), Format::FORMAT_D32_FLOAT, Format::FORMAT_R16_UNORM);

    //const auto& s = getComponent<SpriteComponent>();
    //const auto& tex = std::make_shared<Texture>(mRenderTexture->getShaderResourceView(), Vector2(Window::width(), Window::height()));
    //s->setTexture(tex);
    //auto& t = s->transform();
    //t.setScale(0.5f);
}

void ShadowMap::loadProperties(const rapidjson::Value& inObj) {
    const auto& obj = inObj["shadowMap"];
    if (obj.IsObject()) {
        JsonHelper::getFloat(obj, "lightDistance", &mLightDistance);
        JsonHelper::getFloat(obj, "nearClip", &mNearClip);
        JsonHelper::getFloat(obj, "farClip", &mFarClip);
    }
}

void ShadowMap::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) {
    rapidjson::Value props(rapidjson::kObjectType);
    JsonHelper::setFloat(alloc, &props, "lightDistance", mLightDistance);
    JsonHelper::setFloat(alloc, &props, "nearClip", mNearClip);
    JsonHelper::setFloat(alloc, &props, "farClip", mFarClip);

    inObj.AddMember("shadowMap", props, alloc);
}

void ShadowMap::drawBegin(const Vector3& dirLightDirection) {
    mRenderTexture->drawBegin(1.f, 1.f, 1.f, 1.f, true, false);

    //シェーダー登録
    mDepthTextureCreateShader->setShaderInfo();

    //ライトビュー計算
    mShadowConstBuffer.lightView = Matrix4::createLookAt(-dirLightDirection * mLightDistance, dirLightDirection, Vector3::up);
    mShadowConstBuffer.lightProj = Matrix4::createOrtho(Window::width(), Window::height(), mNearClip, mFarClip);
}

void ShadowMap::draw(const MeshRenderer& renderer) const {
    SimpleMeshConstantBuffer smcb{};
    smcb.wvp = renderer.transform().getWorldTransform() * mShadowConstBuffer.lightView * mShadowConstBuffer.lightProj;
    mDepthTextureCreateShader->transferData(&smcb, sizeof(smcb));

    const auto& meshComp = renderer.getMeshComponent();
    const auto& drawer = meshComp.getDrawer();
    const auto loopCount = meshComp.getMesh()->getMeshCount();
    for (unsigned i = 0; i < loopCount; ++i) {
        drawer->draw(i);
    }
}

void ShadowMap::setShadowConstantBuffer(MeshRenderer& renderer) {
    renderer.getMeshShader().setTransferData(&mShadowConstBuffer, sizeof(mShadowConstBuffer), 2);
}

void ShadowMap::drawEnd() const {
    mRenderTexture->drawEnd();
}

void ShadowMap::transferShadowTexture(unsigned constantBufferIndex) {
    mRenderTexture->transferShaderResourceView(constantBufferIndex);
}

void ShadowMap::drawEndShadowTexture() {
    mRenderTexture->drawEndTexture();
}
