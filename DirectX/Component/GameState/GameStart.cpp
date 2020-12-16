﻿#include "GameStart.h"
#include "../Sprite/SpriteComponent.h"
#include "../UI/CharacterPlacementWarning.h"
#include "../../Device/Subject.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectFactory.h"
#include "../../Input/Input.h"
#include "../../Sprite/SpriteUtility.h"
#include "../../Transform/Transform2D.h"

GameStart::GameStart(GameObject& gameObject)
    : Component(gameObject)
    , mCharacterManager(nullptr)
    , mWarning(nullptr)
    , mStartSprite(nullptr)
    , mCallbackClickSprite(std::make_unique<Subject>())
{
}

GameStart::~GameStart() = default;

void GameStart::start() {
    mWarning = GameObjectCreater::create("CharacterPlacementWarning")->componentManager().getComponent<CharacterPlacementWarning>();
    mStartSprite = getComponent<SpriteComponent>();
}

void GameStart::originalUpdate() {
    //スプライトをクリックしていなければ終了
    if (!clickSprite()) {
        return;
    }

    //キャラクターがいないなら警告して終了
    if (mCharacterManager->getCharacters().empty()) {
        mWarning->enabled();
        return;
    }

    mCallbackClickSprite->notify();
    gameObject().setActive(false);
}

void GameStart::callbackGameStart(const std::function<void()>& callback) {
    mCallbackClickSprite->addObserver(callback);
}

void GameStart::onChangeOperatePhase() {
    //操作フェーズでは使用する
    gameObject().setActive(true);
}

void GameStart::setCharacterManager(const ICharacterManager* manager) {
    mCharacterManager = manager;
}

Vector2 GameStart::getCenterTopPosition() const {
    const auto& t = mStartSprite->transform();
    const auto& currentSize = mStartSprite->getTextureSize() * t.getScale();
    //ピボットが右下と決めつけた上で
    return t.getPosition() + Vector2(-currentSize.x / 2.f, -currentSize.y);
}

bool GameStart::clickSprite() {
    const auto& mouse = Input::mouse();
    //マウスの左ボタンを押してないなら終了
    if (!mouse.getMouseButtonDown(MouseCode::LeftButton)) {
        return false;
    }

    return SpriteUtility::contains(*mStartSprite, mouse.getMousePosition());
}
