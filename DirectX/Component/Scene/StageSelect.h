#pragma once

#include "Scene.h"
#include "../../Math/Math.h"
#include "../../Transform/Pivot.h"
#include <memory>
#include <vector>

class SpriteComponent;
class TextNumber;

class StageSelect : public Scene {
    using SpritePtr = std::shared_ptr<SpriteComponent>;
    using SpriteArray = std::vector<SpritePtr>;
    using TextNumPtr = std::shared_ptr<TextNumber>;
    using TextNumArray = std::vector<TextNumPtr>;

public:
    StageSelect(GameObject& gameObject);
    ~StageSelect();
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;

private:
    StageSelect(const StageSelect&) = delete;
    StageSelect& operator=(const StageSelect&) = delete;

private:
    SpriteArray mSprites;
    TextNumArray mTexts;

    //スプライト描画開始位置
    Vector2 mStartPosition;
    //スプライトの間隔
    Vector2 mNextSpace;
    //スプライトのスケール
    Vector2 mScale;
    //スプライトのピボット
    Pivot mPivot;
};
