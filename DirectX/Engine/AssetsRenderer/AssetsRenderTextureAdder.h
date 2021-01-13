#pragma once

#include "IAddAssets.h"
#include "../../Math/Math.h"
#include <rapidjson/document.h>
#include <memory>
#include <string>

class SpriteButton;

//メッシュを描画したテクスチャ追加クラス
class AssetsRenderTextureAdder {
public:
    AssetsRenderTextureAdder();
    ~AssetsRenderTextureAdder();
    void loadProperties(const rapidjson::Value& inObj);
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const;
    void initialize(IAddAssets* adder);
    void update();
    void draw(const Matrix4& proj);

private:
    AssetsRenderTextureAdder(const AssetsRenderTextureAdder&) = delete;
    AssetsRenderTextureAdder& operator=(const AssetsRenderTextureAdder&) = delete;

    void onClickButton();

private:
    std::unique_ptr<SpriteButton> mButton;
    IAddAssets* mAssetsAdder;
    std::string mSpriteFilePath;
    Vector2 mRenderPosition;
};
