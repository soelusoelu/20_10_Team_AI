#pragma once

#include "../../Component.h"
#include <memory>

class SpriteComponent;

//ステージクリア時の状態管理クラス
class StageClear : public Component {
    using SpritePtr = std::shared_ptr<SpriteComponent>;

public:
    StageClear(GameObject& gameObject);
    ~StageClear();
    virtual void start() override;
    //初期化
    void initialize();
    //プレイヤー側の勝利時
    void onWinPlayerSide();

private:
    StageClear(const StageClear&) = delete;
    StageClear& operator=(const StageClear&) = delete;

private:
    SpritePtr mBackground;
    SpritePtr mStageClear;

    //スプライトの識別番号
    static constexpr int BACKGROUND = 0;
    static constexpr int STAGE_CLEAR = 1;
};
