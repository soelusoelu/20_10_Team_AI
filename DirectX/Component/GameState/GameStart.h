#pragma once

#include "../Component.h"
#include "../../Math/Math.h"
#include <functional>
#include <memory>

class Subject;
class SpriteComponent;

class GameStart : public Component {
public:
    GameStart(GameObject& gameObject);
    ~GameStart();
    virtual void start() override;
    void originalUpdate();
    //アクションフェーズに移行する際のコールバック
    void callbackGameStart(const std::function<void()>& callback);

private:
    GameStart(const GameStart&) = delete;
    GameStart& operator=(const GameStart&) = delete;

    //スタート画像をクリックしたか
    bool clickSprite();

private:
    std::unique_ptr<Subject> mSubject;
    std::shared_ptr<SpriteComponent> mSprite;
};
