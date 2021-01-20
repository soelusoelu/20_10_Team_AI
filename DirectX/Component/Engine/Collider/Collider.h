﻿#pragma once

#include "../../Component.h"
#include <list>
#include <memory>
#include <string>

class Physics;

class Collider : public Component, public std::enable_shared_from_this<Collider> {
    using CollPtr = std::shared_ptr<Collider>;
    using CollPtrList = std::list<CollPtr>;

protected:
    Collider(GameObject& gameObject);
    virtual ~Collider();

public:
    virtual void start() override;
    virtual void lateUpdate() override;
    virtual void finalize() override;
    virtual void drawInspector() override;
    virtual void onEnable(bool value) override;
    //当たり判定を有効化
    void enabled();
    //当たり判定を無効化
    void disabled();
    //当たり判定が有効か
    bool getEnable() const;
    //衝突判定の自動化
    void automation();
    //コライダーを追加
    void addHitCollider(const CollPtr& hit);

    static void setPhysics(Physics* physics);

private:
    //衝突し続けているコライダーを通知する
    void notifyCollisionStay() const;
    //衝突しなくなったコライダーを通知する
    void notifyCollisionExit() const;
    //衝突した瞬間か
    bool isCollisionEnter(const CollPtr& hit) const;
    //衝突し続けているか
    bool isCollisionStay(const CollPtr& hit) const;
    //衝突しなくなったか
    bool isCollisionExit(const CollPtr& hit) const;

protected:
    bool mIsAutoUpdate;
    bool mEnable;

private:
    CollPtrList mPreviousCollider;
    CollPtrList mCurrentCollider;

    static inline Physics* mPhysics = nullptr;
};

