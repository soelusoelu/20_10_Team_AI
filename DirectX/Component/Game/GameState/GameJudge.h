#pragma once

#include "../Character/ICharacterManager.h"
#include "../../Component.h"
#include <functional>
#include <memory>

class Subject;

//ゲームの勝敗判定を行うクラス
class GameJudge : public Component {
public:
    GameJudge(GameObject& gameObject);
    ~GameJudge();

    //キャラクターマネージャーを設定する
    void setCharacterManager(const ICharacterManager* manager);
    //アクションフェーズ移行時の処理
    void onChangeActionPhase();
    //プレイヤー側のキャラクターが死亡したら
    void onDeadPlayerSide();
    //エネミー側のキャラクターが死亡したら
    void onDeadEnemySide();
    //どちらかが勝った際のコールバック
    void callbackSomeWin(const std::function<void()>& callback);
    //プレイヤー側が勝った際のコールバック
    void callbackPlayerWin(const std::function<void()>& callback);
    //エネミー側が勝った際のコールバック
    void callbackEnemyWin(const std::function<void()>& callback);
    //プレイヤー側が勝ったか
    bool isWinPlayerSide() const;

private:
    GameJudge(const GameJudge&) = delete;
    GameJudge& operator=(const GameJudge&) = delete;

private:
    const ICharacterManager* mCharacterManager;
    std::unique_ptr<Subject> mCallbackSomeWin;
    std::unique_ptr<Subject> mCallbackPlayerWin;
    std::unique_ptr<Subject> mCallbackEnemyWin;
    int mPlayerSideCount;
    int mEnemySideCount;
    bool mIsWinPlayerSide;
};
