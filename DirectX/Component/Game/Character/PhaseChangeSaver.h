#pragma once

#include "../../Component.h"
#include "../../../Math/Math.h"
#include <memory>

class HitPointComponent;

//フェーズ移行する際の値保存をするクラス
class PhaseChangeSaver : public Component {
public:
    PhaseChangeSaver(GameObject& gameObject);
    ~PhaseChangeSaver();
    virtual void start() override;

private:
    PhaseChangeSaver(const PhaseChangeSaver&) = delete;
    PhaseChangeSaver& operator=(const PhaseChangeSaver&) = delete;

    //アクションフェーズに移行際の処理
    void onChangeActionPhase();
    //操作フェーズに移行際の処理
    void onChangeOperatePhase();

private:
    std::shared_ptr<HitPointComponent> mHP;
    Vector3 mOperatePosition;
    Quaternion mOperateRotation;
    int mOriginalHP;
};
