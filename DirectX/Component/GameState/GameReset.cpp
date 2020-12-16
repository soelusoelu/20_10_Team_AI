#include "GameReset.h"
#include "../Sprite/SpriteComponent.h"
#include "../../Device/Subject.h"
#include "../../GameObject/GameObject.h"
#include "../../Input/Input.h"
#include "../../Sprite/SpriteUtility.h"

GameReset::GameReset(GameObject& gameObject)
    : Component(gameObject)
    , mCallbackClickSprite(std::make_unique<Subject>())
    , mSprite(nullptr)
{
}

GameReset::~GameReset() = default;

void GameReset::start() {
    mSprite = getComponent<SpriteComponent>();
    //最初は使用しない
    gameObject().setActive(false);
}

void GameReset::originalUpdate() {
    if (SpriteUtility::clickSprite(*mSprite)) {
        mCallbackClickSprite->notify();
        gameObject().setActive(false);
    }
}

void GameReset::callbackGameReset(const std::function<void()>& callback) {
    mCallbackClickSprite->addObserver(callback);
}

void GameReset::onChangeActionPhase() {
    //アクションフェーズでは使用する
    gameObject().setActive(true);
}
