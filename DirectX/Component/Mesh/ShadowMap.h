#pragma once

#include "../Component.h"
#include "../../DirectX/Texture2DDesc.h"
#include "../../System/SystemInclude.h"
#include <memory>

class Shader;
class Texture2D;
class RenderTargetView;
class ShaderResourceView;
class Camera;
class DirectionalLight;

//モデルの影を落とすクラス
class ShadowMap : public Component {
public:
    ShadowMap(GameObject& gameObject);
    ~ShadowMap();
    virtual void awake() override;
    void draw(const Camera& camera, const DirectionalLight& dirLight) const;

private:
    ShadowMap(const ShadowMap&) = delete;
    ShadowMap& operator=(const ShadowMap&) = delete;

    //深度テクスチャ用ディスクリプタを作成する
    void createDepthDesc(Texture2DDesc& desc) const;
    //深度テクスチャを作成する
    void createDepthTexture(const Texture2DDesc& desc);
    //深度テクスチャ用レンダーターゲットビューを作成する
    void createDepthRenderTargetView(const Texture2DDesc& desc);
    //深度テクスチャ用深度ステンシルビューを作成する
    void createDepthStencilView(const Texture2DDesc& desc);
    //深度テクスチャ用シェーダーリソースビューを作成する
    void createDepthShaderResourceView(Format format);

private:
    std::shared_ptr<Shader> mShadowMap;
    std::unique_ptr<Texture2D> mDepthTexture;
    std::unique_ptr<RenderTargetView> mDepthRenderTargetView;
    std::unique_ptr<ShaderResourceView> mDepthShaderResourceView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView;
    int mWidth;
    int mHeight;
};
