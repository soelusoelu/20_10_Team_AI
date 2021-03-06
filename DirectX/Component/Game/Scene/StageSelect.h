﻿#pragma once

#include "../../Engine/Scene/Scene.h"
#include "../../../Math/Math.h"
#include "../../../Transform/Pivot.h"
#include <memory>
#include <string>
#include <vector>

class SpriteComponent;
class TextNumber;

class StageSelect : public Scene {
    using SpritePtr = std::shared_ptr<SpriteComponent>;
    using SpriteArray = std::vector<SpritePtr>;
    using TextNumPtr = std::shared_ptr<TextNumber>;
    using TextNumPtrArray = std::vector<TextNumPtr>;

public:
    StageSelect(GameObject& gameObject);
    ~StageSelect();
    virtual void awake() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;

private:
    StageSelect(const StageSelect&) = delete;
    StageSelect& operator=(const StageSelect&) = delete;

    void selectSprite(const Vector2& mousePosition);
    void changeNewAlpha(int newNo);
    void changeOldAlpha(int oldNo);

private:
    SpriteArray mSprites;
    TextNumPtrArray mTextNums;
    int mSelectingNo;

    //スプライトのファイル名
    std::string mSpriteFileName;
    //スプライト描画開始位置
    Vector2 mStartPosition;
    //スプライトの間隔
    Vector2 mNextSpace;
    //スプライトのスケール
    Vector2 mScale;
    //スプライトのピボット
    Pivot mPivot;
    //スプライト選択時の透明値
    float mSelectingAlpha;
    //操作説明画像を描画するか
    bool mHasDrawOperateSprite;

    //ステージ数
    static constexpr int STAGE_NUM = 3;
    //無効なステージ番号
    static constexpr int INVALID_NO = -1;
    //操作説明画像の番号
    static constexpr int OPERATE_SPRITE_NO = 2;
};
