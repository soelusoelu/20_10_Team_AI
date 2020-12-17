#pragma once

#include "../Component.h"
#include <functional>
#include <memory>

class SpriteButtonComponent;

class GameReset : public Component {
public:
    GameReset(GameObject& gameObject);
    ~GameReset();
    virtual void start() override;

    //アクションからリセットした際のコールバック
    void callbackGameReset(const std::function<void()>& callback);
    //アクションフェーズ移行時の処理
    void onChangeActionPhase();
    //プレイヤー側がステージクリアした際の処理
    void onWinPlayerSide();
    //エネミー側がステージクリアした際の処理
    void onWinEnemySide();

private:
    GameReset(const GameReset&) = delete;
    GameReset& operator=(const GameReset&) = delete;

private:
    std::shared_ptr<SpriteButtonComponent> mButton;
};
