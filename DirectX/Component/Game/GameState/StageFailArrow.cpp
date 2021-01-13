#include "StageFailArrow.h"
#include "../../Engine/Sprite/SpriteComponent.h"
#include "../../../Device/Time.h"
#include "../../../Transform/Transform2D.h"
#include "../../../Utility/LevelLoader.h"

StageFailArrow::StageFailArrow(GameObject& gameObject)
    : Component(gameObject)
    , mDownArrow(nullptr)
    , mStandardPosition(Vector2::zero)
    , mOffsetY(0.f)
    , mMoveSpeed(0.f)
    , mSwingHeight(0.f)
    , mSinValue(0.f)
{
}

StageFailArrow::~StageFailArrow() = default;

void StageFailArrow::start() {
    mDownArrow = getComponent<SpriteComponent>();
}

void StageFailArrow::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getFloat(inObj, "offsetY", &mOffsetY);
    JsonHelper::getFloat(inObj, "moveSpeed", &mMoveSpeed);
    JsonHelper::getFloat(inObj, "swingHeight", &mSwingHeight);
}

void StageFailArrow::originalUpdate() {
    mSinValue += mMoveSpeed * Time::deltaTime;
    if (mSinValue > 360.f) {
        mSinValue -= 360.f;
    }

    //基準位置 + サイン波での振り幅
    mDownArrow->transform().setPosition(mStandardPosition + Vector2::up * Math::abs(Math::sin(mSinValue)) * mSwingHeight);
}

void StageFailArrow::setPosition(const Vector2& pos) {
    mStandardPosition = pos + Vector2::up * mOffsetY;
}
