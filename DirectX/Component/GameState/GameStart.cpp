#include "GameStart.h"
#include "../Sprite/SpriteComponent.h"
#include "../../Device/Subject.h"
#include "../../GameObject/GameObject.h"
#include "../../Input/Input.h"
#include "../../Sprite/SpriteUtility.h"

GameStart::GameStart(GameObject& gameObject)
    : Component(gameObject)
    , mSubject(std::make_unique<Subject>())
{
}

GameStart::~GameStart() = default;

void GameStart::start() {
    mSprite = getComponent<SpriteComponent>();
}

void GameStart::originalUpdate() {
    if (clickSprite()) {
        mSubject->notify();
        gameObject().setActive(false);
    }
}

void GameStart::callbackGameStart(const std::function<void()>& callback) {
    mSubject->addObserver(callback);
}

bool GameStart::clickSprite() {
    const auto& mouse = Input::mouse();
    //マウスの左ボタンを押してないなら終了
    if (!mouse.getMouseButtonDown(MouseCode::LeftButton)) {
        return false;
    }

    return SpriteUtility::contains(*mSprite, mouse.getMousePosition());
}
