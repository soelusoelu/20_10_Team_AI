#pragma once

#include "../Component.h"
#include "../../Math/Math.h"
#include <memory>
#include <vector>

class SpriteComponent;

//ステージクリア時の状態管理クラス
class GameClear : public Component {
    using SpritePtr = std::shared_ptr<SpriteComponent>;

public:
    GameClear(GameObject& gameObject);
    ~GameClear();
    virtual void start() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;

    //初期化
    void initialize();
    //プレイヤー側の勝利時のアップデート

    //エネミー側の勝利時のアップデート
    void updateEnemyWin();
    //下向き矢印の基準位置を設定する
    void setPositionForDownArrow(const Vector2& pos);
    //プレイヤー側の勝利時
    void onWinPlayerSide();
    //エネミー側の勝利時
    void onWinEnemySide();

private:
    GameClear(const GameClear&) = delete;
    GameClear& operator=(const GameClear&) = delete;

private:
    SpritePtr mBackground;
    SpritePtr mDownArrow;
    Vector2 mDownArrowStandardPosition;
    float mDownArrowOffsetY;
    float mDownArrowMoveSpeed;
    float mDownArrowSwingHeight;
    float mDownArrowSinValue;

    //スプライトの識別番号
    static constexpr int PLAYER_WIN_BACKGROUND = 0;
    static constexpr int ENEMY_WIN_DOWN_ARROW = 1;
};
