#include "Sprite3D.h"
#include "../../DebugLayer/ImGuiWrapper.h"
#include "../../DirectX/DirectX.h"
#include "../../GameObject/GameObject.h"
#include "../../Imgui/imgui.h"
#include "../../Sprite/SpriteManager.h"
#include "../../System/AssetsManager.h"
#include "../../System/Window.h"
#include "../../System/Shader/ConstantBuffers.h"
#include "../../System/Shader/Shader.h"
#include "../../System/Texture/Texture.h"
#include "../../System/Texture/TextureFromFile.h"
#include "../../Transform/Transform3D.h"
#include "../../Utility/LevelLoader.h"
#include <cassert>

Sprite3D::Sprite3D(GameObject& gameObject) :
    Component(gameObject),
    mTransform(std::make_unique<Transform3D>()),
    mTexture(nullptr),
    mShader(nullptr),
    mTextureAspect(Vector2::zero),
    mColor(ColorPalette::white),
    mAlpha(1.f),
    mUV(Vector4(0.f, 0.f, 1.f, 1.f)),
    mFileName(""),
    mIsActive(true),
    mIsBillboard(false) {
}

Sprite3D::~Sprite3D() = default;

void Sprite3D::awake() {
    //ファイル名を読み込めてたらテクスチャを生成
    if (!mFileName.empty()) {
        mTexture = AssetsManager::instance().createTexture(mFileName);
    }

    //シェーダー生成
    mShader = AssetsManager::instance().createShader("Texture3D.hlsl");

    //テクスチャのアスペクト比を計算
    calcAspectRatio();

    //マネージャーに登録する
    addToManager();
}

void Sprite3D::lateUpdate() {
    if (getActive()) {
        mTransform->computeWorldTransform();
    }
}

void Sprite3D::finalize() {
    destroy();
}

void Sprite3D::onEnable(bool value) {
    setActive(value);
}

void Sprite3D::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getString(inObj, "fileName", &mFileName);
    JsonHelper::getBool(inObj, "isActive", &mIsActive);
    JsonHelper::getBool(inObj, "isBillboard", &mIsBillboard);
    Vector3 vec3;
    if (JsonHelper::getVector3(inObj, "position", &vec3)) {
        mTransform->setPosition(vec3);
    }
    if (JsonHelper::getVector3(inObj, "rotation", &vec3)) {
        mTransform->setRotation(vec3);
    }
    if (JsonHelper::getVector3(inObj, "scale", &vec3)) {
        mTransform->setScale(vec3);
    }
    JsonHelper::getVector3(inObj, "color", &mColor);
    JsonHelper::getFloat(inObj, "alpha", &mAlpha);
    JsonHelper::getVector4(inObj, "uv", &mUV);
}

void Sprite3D::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const {
    JsonHelper::setString(alloc, inObj, "fileName", mFileName);
    JsonHelper::setBool(alloc, inObj, "isActive", mIsActive);
    JsonHelper::setBool(alloc, inObj, "isBillboard", mIsBillboard);
    JsonHelper::setVector3(alloc, inObj, "position", mTransform->getPosition());
    JsonHelper::setVector3(alloc, inObj, "rotation", mTransform->getRotation().euler());
    JsonHelper::setVector3(alloc, inObj, "scale", mTransform->getScale());
    JsonHelper::setVector3(alloc, inObj, "color", mColor);
    JsonHelper::setFloat(alloc, inObj, "alpha", mAlpha);
    JsonHelper::setVector4(alloc, inObj, "uv", mUV);
}

void Sprite3D::drawInspector() {
    ImGui::Text("FileName: %s", mFileName.c_str());
    mTransform->drawInspector();
    ImGuiWrapper::colorEdit3("Color", mColor);
    ImGuiWrapper::sliderFloat("Alpha", mAlpha, 0.f, 1.f);
    ImGuiWrapper::sliderVector4("UV", mUV, 0.f, 1.f);
}

void Sprite3D::draw(const Matrix4& viewProj) const {
    if (!mTexture) {
        return;
    }

    //シェーダーを登録
    mShader->setShaderInfo();
    //テクスチャーを登録
    mTexture->setTextureInfo();

    //シェーダーのコンスタントバッファーに各種データを渡す
    TextureConstantBuffer cb;
    cb.wp = mTransform->getWorldTransform() * viewProj;
    cb.color = Vector4(mColor, mAlpha);
    cb.uv = mUV;

    //シェーダーにデータ転送
    mShader->transferData(&cb, sizeof(cb));

    //プリミティブをレンダリング
    MyDirectX::DirectX::instance().drawIndexed(6);
}

void Sprite3D::drawBillboard(const Matrix4& invView, const Matrix4& viewProj) {
    if (!mTexture) {
        return;
    }

    //シェーダーを登録
    mShader->setShaderInfo();
    //テクスチャーを登録
    mTexture->setTextureInfo();

    //シェーダーのコンスタントバッファーに各種データを渡す
    TextureConstantBuffer cb;
    //ワールド、射影行列を渡す
    cb.wp = invView * mTransform->getWorldTransform() * viewProj;
    cb.color = Vector4(mColor, mAlpha);
    cb.uv = mUV;

    //シェーダーにデータ転送
    mShader->transferData(&cb, sizeof(cb));

    //プリミティブをレンダリング
    MyDirectX::DirectX::instance().drawIndexed(6);
}

Transform3D& Sprite3D::transform() const {
    return *mTransform;
}

void Sprite3D::setColor(const Vector3& color) {
    mColor = color;
}

void Sprite3D::setColor(float r, float g, float b) {
    mColor.x = r;
    mColor.y = g;
    mColor.z = b;
}

const Vector3& Sprite3D::getColor() const {
    return mColor;
}

void Sprite3D::setAlpha(float alpha) {
    mAlpha = alpha;
}

float Sprite3D::getAlpha() const {
    return mAlpha;
}

void Sprite3D::setUV(float l, float t, float r, float b) {
    //assert(0.f <= l || l <= 1.f);
    //assert(0.f <= t || t <= 1.f);
    //assert(l <= r || r <= 1.f);
    //assert(t <= b || b <= 1.f);

    mUV.x = l;
    mUV.y = t;
    mUV.z = r;
    mUV.w = b;
}

const Vector4& Sprite3D::getUV() const {
    return mUV;
}

void Sprite3D::setActive(bool value) {
    mIsActive = value;
}

bool Sprite3D::getActive() const {
    return mIsActive;
}

void Sprite3D::setTextureFromFileName(const std::string& fileName) {
    if (mTexture) {
        mTexture.reset();
    }
    mTexture = AssetsManager::instance().createTexture(fileName);

    //テクスチャのアスペクト比を計算
    calcAspectRatio();

    //ファイル名変更
    mFileName = fileName;
}

const Texture& Sprite3D::texture() const {
    return *mTexture;
}

const Vector2& Sprite3D::getTextureAspect() const {
    return mTextureAspect;
}

const Shader& Sprite3D::shader() const {
    return *mShader;
}

const std::string& Sprite3D::fileName() const {
    return mFileName;
}

void Sprite3D::setBillboard(bool value) {
    mIsBillboard = value;
}

bool Sprite3D::isBillboard() const {
    return mIsBillboard;
}

void Sprite3D::setSpriteManager(SpriteManager* manager) {
    mSpriteManager = manager;
}

void Sprite3D::addToManager() {
    if (mSpriteManager) {
        mSpriteManager->add3D(shared_from_this());
    }
}

void Sprite3D::calcAspectRatio() {
    if (!mTexture) {
        return;
    }

    const auto& texSize = mTexture->getTextureSize();
    mTextureAspect = Vector2::one;
    if (texSize.x > texSize.y) {
        mTextureAspect.x = texSize.x / texSize.y;
    } else {
        mTextureAspect.y = texSize.y / texSize.x;
    }
    mTransform->setScale(mTransform->getScale() * Vector3(mTextureAspect, 1.f));
}
