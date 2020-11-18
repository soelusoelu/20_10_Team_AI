#pragma once

#include "GameState.h"
#include "../Component.h"
#include "../../Math/Math.h"
#include <memory>

class SpriteComponent;

class GameStart : public Component {
public:
    GameStart(GameObject& gameObject);
    ~GameStart();
    virtual void start() override;
    void originalUpdate(GameState& state);

private:
    GameStart(const GameStart&) = delete;
    GameStart& operator=(const GameStart&) = delete;

    //スタート画像をクリックしたか
    bool clickSprite();

private:
    std::shared_ptr<SpriteComponent> mSprite;
};
