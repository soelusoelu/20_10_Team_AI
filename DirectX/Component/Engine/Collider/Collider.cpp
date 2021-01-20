#include "Collider.h"
#include "../../../Device/Physics.h"
#include "../../../GameObject/GameObject.h"
#include "../../../Imgui/imgui.h"
#include <algorithm>

Collider::Collider(GameObject& gameObject) :
    Component(gameObject),
    mIsAutoUpdate(true),
    mEnable(false) {
}

Collider::~Collider() = default;

void Collider::start() {
    if (mPhysics) {
        mPhysics->add(shared_from_this());
        mEnable = true;
    }
}

void Collider::lateUpdate() {
    for (const auto& curColl : mCurrentCollider) {
        if (isCollisionStay(curColl)) {
            gameObject().componentManager().onCollisionStay(*curColl);
        }
    }
    for (const auto& preColl : mPreviousCollider) {
        if (isCollisionExit(preColl)) {
            gameObject().componentManager().onCollisionExit(*preColl);
        }
    }

    mPreviousCollider.resize(mCurrentCollider.size());
    std::copy(mCurrentCollider.begin(), mCurrentCollider.end(), mPreviousCollider.begin());
    mCurrentCollider.clear();
}

void Collider::finalize() {
    mPreviousCollider.clear();
    mCurrentCollider.clear();

    if (mPhysics) {
        mPhysics->remove(shared_from_this());
    }
}

void Collider::drawInspector() {
    ImGui::Checkbox("IsAutoUpdate", &mIsAutoUpdate);
    ImGui::Checkbox("Enable", &mEnable);
}

void Collider::onEnable(bool value) {
    (value) ? enabled() : disabled();
}

void Collider::enabled() {
    mEnable = true;
}

void Collider::disabled() {
    mEnable = false;
}

bool Collider::getEnable() const {
    return mEnable;
}

void Collider::automation() {
    if (!mIsAutoUpdate) {
        mIsAutoUpdate = true;
    }
}

void Collider::addHitCollider(const CollPtr& hit) {
    mCurrentCollider.emplace_back(hit);

    //追加されたヒットコライダーが新しいものなら
    if (isCollisionEnter(hit)) {
        gameObject().componentManager().onCollisionEnter(*hit);
    }
}

void Collider::setPhysics(Physics* physics) {
    mPhysics = physics;
}

bool Collider::isCollisionEnter(const CollPtr& hit) const {
    auto itr = std::find(mPreviousCollider.begin(), mPreviousCollider.end(), hit);
    return (itr == mPreviousCollider.end());
}

bool Collider::isCollisionStay(const CollPtr& hit) const {
    auto itr = std::find(mPreviousCollider.begin(), mPreviousCollider.end(), hit);
    return (itr != mPreviousCollider.end());
}

bool Collider::isCollisionExit(const CollPtr& hit) const {
    auto itr = std::find(mCurrentCollider.begin(), mCurrentCollider.end(), hit);
    return (itr == mCurrentCollider.end());
}
