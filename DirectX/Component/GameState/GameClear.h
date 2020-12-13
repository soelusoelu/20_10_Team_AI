#pragma once

#include "../Component.h"
#include <memory>

class SpriteComponent;

//ステージクリア時の状態管理クラス
class GameClear : public Component {
public:
    GameClear(GameObject& gameObject);
    ~GameClear();
    virtual void start() override;
    //プレイヤー側の勝利時
    void onWinPlayerSide();
    //エネミー側の勝利時
    void onWinEnemySide();

private:
    GameClear(const GameClear&) = delete;
    GameClear& operator=(const GameClear&) = delete;

private:
    std::shared_ptr<SpriteComponent> mBackground;
};
