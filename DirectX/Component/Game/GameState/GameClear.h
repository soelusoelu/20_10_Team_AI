#pragma once

#include "../../Component.h"
#include <memory>

class StageClear;
class StageFail;
class ClearToStageSelect;

//ステージクリア時の状態管理クラス
class GameClear : public Component {
public:
    GameClear(GameObject& gameObject);
    ~GameClear();
    virtual void start() override;

    //初期化
    void initialize();
    //プレイヤー側の勝利時のアップデート
    void updatePlayerWin();
    //エネミー側の勝利時のアップデート
    void updateEnemyWin();
    //プレイヤー側の勝利時
    void onWinPlayerSide();
    //エネミー側の勝利時
    void onWinEnemySide();
    StageClear& getStageClear() const;
    StageFail& getStageFail() const;

private:
    GameClear(const GameClear&) = delete;
    GameClear& operator=(const GameClear&) = delete;

private:
    std::shared_ptr<StageClear> mStageClear;
    std::shared_ptr<StageFail> mStageFail;
    std::shared_ptr<ClearToStageSelect> mClearToStageSelect;
};
