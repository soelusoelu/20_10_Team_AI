#include "GameStart.h"
#include "../Sprite/SpriteComponent.h"
#include "../../Device/Subject.h"
#include "../../GameObject/GameObject.h"
#include "../../Input/Input.h"
#include "../../Sprite/SpriteUtility.h"

GameStart::GameStart(GameObject& gameObject)
    : Component(gameObject)
    , mCallbackClickSprite(std::make_unique<Subject>())
    , mSprite(nullptr)
{
}

GameStart::~GameStart() = default;

void GameStart::start() {
    mSprite = getComponent<SpriteComponent>();
}

void GameStart::originalUpdate() {
    if (clickSprite()) {
        mCallbackClickSprite->notify();
        gameObject().setActive(false);
    }
}

void GameStart::callbackGameStart(const std::function<void()>& callback) {
    mCallbackClickSprite->addObserver(callback);
}

void GameStart::onChangeOperatePhase() {
    //操作フェーズでは使用する
    gameObject().setActive(true);
}

bool GameStart::clickSprite() {
    const auto& mouse = Input::mouse();
    //マウスの左ボタンを押してないなら終了
    if (!mouse.getMouseButtonDown(MouseCode::LeftButton)) {
        return false;
    }

    return SpriteUtility::contains(*mSprite, mouse.getMousePosition());
}
