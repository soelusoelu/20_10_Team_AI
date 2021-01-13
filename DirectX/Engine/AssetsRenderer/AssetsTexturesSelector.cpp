#include "AssetsTexturesSelector.h"
#include "AssetsRenderTexture.h"
#include "../../Device/Subject.h"
#include "../../Input/Input.h"
#include "../../Sprite/SpriteUtility.h"
#include "../../System/Texture/MeshRenderOnTexture.h"

AssetsTexturesSelector::AssetsTexturesSelector()
    : mTexturesGetter(nullptr)
    , mCallbackSelectTexture(std::make_unique<Subject>())
{
}

AssetsTexturesSelector::~AssetsTexturesSelector() = default;

void AssetsTexturesSelector::callbackSelectTexture(const std::function<void()>& callback) {
    mCallbackSelectTexture->addObserver(callback);
}

void AssetsTexturesSelector::initialize(const IAssetsRenderTexturesGetter* getter) {
    mTexturesGetter = getter;
}

bool AssetsTexturesSelector::selectTexture(AssetsRenderTexturePtr& out) {
    const auto& mouse = Input::mouse();

    //マウスの左ボタンを押していなければ終了
    if (!mouse.getMouseButtonDown(MouseCode::LeftButton)) {
        return false;
    }

    //全テクスチャからマウス座標が含まれているのを探す
    const auto& textures = mTexturesGetter->getTextures();
    const auto& mousePos = Input::mouse().getMousePosition();
    for (const auto& tex : textures) {
        if (SpriteUtility::containsDebug(tex->getTexture().getSprite(), mousePos)) {
            out = tex;
            mCallbackSelectTexture->notify();
            return true;
        }
    }

    return false;
}
