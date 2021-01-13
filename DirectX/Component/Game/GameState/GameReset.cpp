#include "GameReset.h"
#include "../../Engine/Sprite/SpriteButtonComponent.h"
#include "../../../GameObject/GameObject.h"

GameReset::GameReset(GameObject& gameObject)
    : Component(gameObject)
    , mButton(nullptr)
{
}

GameReset::~GameReset() = default;

void GameReset::start() {
    mButton = getComponent<SpriteButtonComponent>();
    callbackGameReset([&] { gameObject().setActive(false); });

    //最初は使用しない
    gameObject().setActive(false);
}

void GameReset::callbackGameReset(const std::function<void()>& callback) {
    mButton->callbackClick(callback);
}

void GameReset::onChangeActionPhase() {
    //アクションフェーズでは使用する
    gameObject().setActive(true);
}

void GameReset::onWinPlayerSide() {
    mButton->enableButtonFunction(false);
}

void GameReset::onWinEnemySide() {
    mButton->enableButtonFunction(true);
}
