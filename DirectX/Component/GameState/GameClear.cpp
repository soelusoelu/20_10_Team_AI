#include "GameClear.h"
#include "../Sprite/SpriteComponent.h"
#include "../../Device/Time.h"
#include "../../GameObject/GameObject.h"
#include "../../Transform/Transform2D.h"
#include "../../Utility/LevelLoader.h"

GameClear::GameClear(GameObject& gameObject)
    : Component(gameObject)
    , mBackground(nullptr)
    , mDownArrow(nullptr)
    , mDownArrowStandardPosition(Vector2::zero)
    , mDownArrowOffsetY(0.f)
    , mDownArrowMoveSpeed(0.f)
    , mDownArrowSwingHeight(0.f)
    , mDownArrowSinValue(0.f)
{
}

GameClear::~GameClear() = default;

void GameClear::start() {
    const auto& sprites = getComponents<SpriteComponent>();
    mBackground = sprites[PLAYER_WIN_BACKGROUND];
    mDownArrow = sprites[ENEMY_WIN_DOWN_ARROW];
    //最初は使用しない
    gameObject().setActive(false);
}

void GameClear::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getFloat(inObj, "downArrowOffsetY", &mDownArrowOffsetY);
    JsonHelper::getFloat(inObj, "downArrowMoveSpeed", &mDownArrowMoveSpeed);
    JsonHelper::getFloat(inObj, "downArrowSwingHeight", &mDownArrowSwingHeight);
}

void GameClear::initialize() {
    gameObject().setActive(false);
}

void GameClear::updateEnemyWin() {
    mDownArrowSinValue += mDownArrowMoveSpeed * Time::deltaTime;
    if (mDownArrowSinValue > 360.f) {
        mDownArrowSinValue -= 360.f;
    }

    //基準位置 + サイン波での振り幅
    mDownArrow->transform().setPosition(mDownArrowStandardPosition + Vector2::up * fabsf(Math::sin(mDownArrowSinValue)) * mDownArrowSwingHeight);
}

void GameClear::setPositionForDownArrow(const Vector2& pos) {
    mDownArrowStandardPosition = pos + Vector2::up * mDownArrowOffsetY;
}

void GameClear::onWinPlayerSide() {
    gameObject().setActive(true);
    mDownArrow->setActive(false);
}

void GameClear::onWinEnemySide() {
    gameObject().setActive(true);
    mBackground->setActive(false);
}
