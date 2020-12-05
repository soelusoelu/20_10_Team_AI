﻿#include "ShadowMap.h"
#include "MeshComponent.h"
#include "MeshRenderer.h"
#include "MeshShader.h"
#include "../Camera/Camera.h"
#include "../Light/DirectionalLight.h"
#include "../../DirectX/DirectXInclude.h"
#include "../../Math/Math.h"
#include "../../System/AssetsManager.h"
#include "../../System/Window.h"
#include "../../System/Shader/Shader.h"
#include "../../Transform/Transform3D.h"

ShadowMap::ShadowMap(GameObject& gameObject)
    : Component(gameObject)
    , mDepthTextureCreateShader(AssetsManager::instance().createShader("ShadowDepthTextureCreater.hlsl"))
    , mDepthTexture(nullptr)
    , mDepthRenderTargetView(nullptr)
    , mDepthStencilView(nullptr)
    , mDepthShaderResourceView(nullptr)
    , mWidth(0)
    , mHeight(0)
{
#ifdef _DEBUG
    mWidth = Window::debugWidth();
    mHeight = Window::debugHeight();
#else
    mWidth = Window::width();
    mHeight = Window::height();
#endif // _DEBUG
}

ShadowMap::~ShadowMap() = default;

void ShadowMap::awake() {
    Texture2DDesc desc{};
    createDepthDesc(desc);
    createDepthTexture(desc);
    createDepthRenderTargetView(desc);
    createDepthStencilView(desc);
    createDepthShaderResourceView(desc.format);
}

void ShadowMap::drawBegin() const {
    //レンダーターゲットを設定する
    mDepthRenderTargetView->setRenderTarget(mDepthStencilView.Get());
    //レンダーターゲットをクリアする
    mDepthRenderTargetView->clearRenderTarget();
    //深度バッファクリア
    MyDirectX::DirectX::instance().deviceContext()->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);

    //シェーダー登録
    mDepthTextureCreateShader->setShaderInfo();
}

void ShadowMap::draw(const MeshRenderer& renderer, const Camera& camera, const DirectionalLight& dirLight) const {
    SimpleMeshConstantBuffer smcb{};
    smcb.wvp = renderer.transform().getWorldTransform() * camera.getViewProjection();
    mDepthTextureCreateShader->transferData(&smcb, sizeof(smcb));

    const auto& meshComp = renderer.getMeshComponent();
    const auto& drawer = meshComp.getDrawer();
    const auto loopCount = meshComp.getMesh()->getMeshCount();
    for (unsigned i = 0; i < loopCount; ++i) {
        drawer->draw(i);
    }
}

void ShadowMap::setShadowConstantBuffer(MeshRenderer& renderer, const Camera& camera, const DirectionalLight& dirLight) {
    mShadowConstBuffer.lightView = Matrix4::createLookAt(Vector3::up * 100.f, dirLight.getDirection() * 100.f, Vector3::up);
    mShadowConstBuffer.lightProj = Matrix4::createPerspectiveFOV(mWidth, mHeight, camera.getFov(), camera.getNearClip(), camera.getFarClip());

    renderer.getMeshShader().setTransferData(&mShadowConstBuffer, sizeof(mShadowConstBuffer), 2);
}

void ShadowMap::drawEnd() const {
    auto& dx = MyDirectX::DirectX::instance();

    //レンダーターゲットをバックバッファに戻す
    dx.setRenderTarget();
    //バックバッファをクリアする
    //dx.clearRenderTarget();
    //深度バッファクリア
    //dx.clearDepthStencilView();
}

void ShadowMap::createDepthDesc(Texture2DDesc& desc) const {
    //深度テクスチャ用ディスクリプタ
    desc.width = mWidth;
    desc.height = mHeight;
    desc.format = Format::FORMAT_R32_FLOAT;
    desc.usage = Usage::USAGE_DEFAULT;
    desc.bindFlags =
        static_cast<unsigned>(Texture2DBind::TEXTURE_BIND_RENDER_TARGET)
        | static_cast<unsigned>(Texture2DBind::TEXTURE_BIND_SHADER_RESOURCE);
}

void ShadowMap::createDepthTexture(const Texture2DDesc& desc) {
    //深度テクスチャ作成
    mDepthTexture = std::make_unique<Texture2D>(desc);
}

void ShadowMap::createDepthRenderTargetView(const Texture2DDesc& desc) {
    //深度テクスチャ用レンダーターゲットビュー
    RenderTargetViewDesc rtvDesc{};
    rtvDesc.format = desc.format;

    //深度テクスチャ用レンダーターゲットビュー作成
    mDepthRenderTargetView = std::make_unique<RenderTargetView>(*mDepthTexture, &rtvDesc);
}

void ShadowMap::createDepthStencilView(const Texture2DDesc& desc) {
    //深度テクスチャ用深度ステンシルビュー
    //バインドフラグ以外深度テクスチャと一緒
    Texture2DDesc dsDesc{};
    memcpy_s(&dsDesc, sizeof(dsDesc), &desc, sizeof(desc));
    dsDesc.format = Format::FORMAT_D24_UNORM_S8_UINT;
    dsDesc.bindFlags = static_cast<unsigned>(Texture2DBind::TEXTURE_BIND_DEPTH_STENCIL);
    auto depthStencilTexture = std::make_unique<Texture2D>(dsDesc);

    //深度テクスチャ用深度ステンシルビュー作成
    MyDirectX::DirectX::instance().device()->CreateDepthStencilView(depthStencilTexture->texture2D(), nullptr, &mDepthStencilView);
}

void ShadowMap::createDepthShaderResourceView(Format format) {
    //深度テクスチャ用シェーダーリソースビュー
    ShaderResourceViewDesc srvDesc{};
    srvDesc.format = format;

    //深度テクスチャ用シェーダーリソースビュー作成
    mDepthShaderResourceView = std::make_unique<ShaderResourceView>(*mDepthTexture, &srvDesc);
}
