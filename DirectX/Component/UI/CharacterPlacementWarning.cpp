#include "CharacterPlacementWarning.h"
#include "../Sprite/SpriteComponent.h"
#include "../../GameObject/GameObject.h"
#include "../../Utility/Easing.h"
#include "../../Utility/LevelLoader.h"

CharacterPlacementWarning::CharacterPlacementWarning(GameObject& gameObject)
    : Component(gameObject)
    , mSprite(nullptr)
    , mEasingSpeed(0.f)
    , mEaseValue(0.f)
{
}

CharacterPlacementWarning::~CharacterPlacementWarning() = default;

void CharacterPlacementWarning::start() {
    mSprite = getComponent<SpriteComponent>();
    gameObject().setActive(false);
}

void CharacterPlacementWarning::update() {
    //スプライトの透明度を下げる
    mSprite->setAlpha(1.f - Easing::easeInBack(mEaseValue));

    mEaseValue += mEasingSpeed;

    //イージングが終了したら非アクティブにする
    if (mEaseValue >= 1.f) {
        gameObject().setActive(false);
    }
}

void CharacterPlacementWarning::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getFloat(inObj, "easingSpeed", &mEasingSpeed);
}

void CharacterPlacementWarning::enabled() {
    gameObject().setActive(true);
    mSprite->setAlpha(1.f);
    mEaseValue = 0.f;
}
