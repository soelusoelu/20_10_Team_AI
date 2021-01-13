#pragma once

#include "../../Component.h"
#include "../../../Math/Math.h"
#include <memory>

class SpriteComponent;
class StageFailArrow;

//ステージ失敗時の状態管理クラス
class StageFail : public Component {
public:
    StageFail(GameObject& gameObject);
    ~StageFail();
    virtual void start() override;
    void originalUpdate();
    //初期化
    void initialize();
    //エネミー側の勝利時
    void onWinEnemySide();
    StageFailArrow& getFailArrow() const;

private:
    StageFail(const StageFail&) = delete;
    StageFail& operator=(const StageFail&) = delete;

private:
    std::shared_ptr<StageFailArrow> mArrow;
};
