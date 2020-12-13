#include "GameClear.h"
#include "../Sprite/SpriteComponent.h"
#include "../../GameObject/GameObject.h"

GameClear::GameClear(GameObject& gameObject)
    : Component(gameObject)
    , mBackground(nullptr)
{
}

GameClear::~GameClear() = default;

void GameClear::start() {
    mBackground = getComponent<SpriteComponent>();
    //最初は使用しない
    gameObject().setActive(false);
}

void GameClear::onWinPlayerSide() {
    gameObject().setActive(true);
}

void GameClear::onWinEnemySide() {
    gameObject().setActive(true);
}
