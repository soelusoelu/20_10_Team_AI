#pragma once

#include "../../Component.h"
#include "../../../Math/Math.h"
#include <memory>
#include <string>

class HitPointComponent;
class AABBCollider;
class Sprite3D;

//HP表示クラス
class HitPointRenderer : public Component {
public:
    HitPointRenderer(GameObject& gameObject);
    ~HitPointRenderer();
    virtual void awake() override;
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;

private:
    HitPointRenderer(const HitPointRenderer&) = delete;
    HitPointRenderer& operator=(const HitPointRenderer&) = delete;

    //アクションフェーズに変わった際の処理
    void onChangeActionPhase();
    //操作フェーズに変わった際の処理
    void onChangeOperatePhase();
    //HPが更新されたら
    void onUpdateHp();

private:
    std::shared_ptr<HitPointComponent> mHp;
    std::shared_ptr<AABBCollider> mCollider;
    std::shared_ptr<Sprite3D> mCurrentHp;
    std::shared_ptr<Sprite3D> mLostHp;
    std::string mCurrentHpFileName;
    std::string mLostHpFileName;
    float mBarPivotY;
    Vector2 mBarScale;
};
