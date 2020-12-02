#pragma once

#include "../Component.h"
#include <functional>
#include <memory>

class Subject;
class SpriteComponent;

class GameReset : public Component {
public:
    GameReset(GameObject& gameObject);
    ~GameReset();
    virtual void start() override;
    void originalUpdate();
    //アクションからリセットした際のコールバック
    void callbackGameReset(const std::function<void()>& callback);
    //アクションフェーズ移行時の処理
    void onChangeActionPhase();

private:
    GameReset(const GameReset&) = delete;
    GameReset& operator=(const GameReset&) = delete;

    //画像をクリックしたか
    bool clickSprite();

private:
    std::unique_ptr<Subject> mCallbackClickSprite;
    std::shared_ptr<SpriteComponent> mSprite;
};
