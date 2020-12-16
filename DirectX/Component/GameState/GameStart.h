#pragma once

#include "../Component.h"
#include "../Character/ICharacterManager.h"
#include "../../Math/Math.h"
#include <functional>
#include <memory>

class CharacterPlacementWarning;
class SpriteComponent;
class Subject;

class GameStart : public Component {
public:
    GameStart(GameObject& gameObject);
    ~GameStart();
    virtual void start() override;
    void originalUpdate();
    //アクションフェーズに移行する際のコールバック
    void callbackGameStart(const std::function<void()>& callback);
    //操作フェーズ移行時の処理
    void onChangeOperatePhase();
    //キャラクターマネージャーをセットする
    void setCharacterManager(const ICharacterManager* manager);
    //スプライトのセンターアップ位置
    Vector2 getCenterTopPosition() const;

private:
    GameStart(const GameStart&) = delete;
    GameStart& operator=(const GameStart&) = delete;

private:
    const ICharacterManager* mCharacterManager;
    std::shared_ptr<CharacterPlacementWarning> mWarning;
    std::shared_ptr<SpriteComponent> mStartSprite;
    std::unique_ptr<Subject> mCallbackClickSprite;
};
