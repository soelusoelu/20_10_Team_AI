#pragma once

#include "Scene.h"
#include "../../Math/Math.h"
#include "../../Transform/Pivot.h"
#include <memory>
#include <string>
#include <vector>

class SpriteComponent;
class Text;

class Title : public Scene {
    using SpritePtr = std::shared_ptr<SpriteComponent>;
    using SpriteArray = std::vector<SpritePtr>;
    using TextPtr = std::shared_ptr<Text>;
    using TextArray = std::vector<TextPtr>;

public:
    Title(GameObject& gameObject);
    ~Title();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;

private:
    Title(const Title&) = delete;
    Title& operator=(const Title&) = delete;

    void selectSprite(const Vector2& mousePosition);
    void changeNewAlpha(int newNo);
    void changeOldAlpha(int oldNo);

private:
    SpriteArray mSprites;
    TextArray mTexts;
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

    //ゲームスタートスプライトの番号
    static constexpr int GAME_START_SPRITE_NO = 2;
    //無効なステージ番号
    static constexpr int INVALID_NO = -1;
};
