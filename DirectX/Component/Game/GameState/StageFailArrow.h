#pragma once

#include "../../Component.h"
#include "../../../Math/Math.h"
#include <memory>

class SpriteComponent;

//ステージ失敗時の矢印スプライト操作クラス
class StageFailArrow : public Component {
public:
    StageFailArrow(GameObject& gameObject);
    ~StageFailArrow();
    virtual void start() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    void originalUpdate();
    //矢印の基準位置を設定する
    void setPosition(const Vector2& pos);

private:
    StageFailArrow(const StageFailArrow&) = delete;
    StageFailArrow& operator=(const StageFailArrow&) = delete;

private:
    std::shared_ptr<SpriteComponent> mDownArrow;
    Vector2 mStandardPosition;
    float mOffsetY;
    float mMoveSpeed;
    float mSwingHeight;
    float mSinValue;
};
