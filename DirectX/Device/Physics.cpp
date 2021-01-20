#include "Physics.h"
#include "../Collision/Collision.h"
#include "../Component/ComponentManager.h"
#include "../Component/Engine/Collider/AABBCollider.h"
#include <algorithm>

Physics::Physics() {
    Collider::setPhysics(this);
}

Physics::~Physics() {
    Collider::setPhysics(nullptr);
}

void Physics::add(const CollPtr& collider) {
    mColliders.emplace_back(collider);
}

void Physics::remove(const CollPtr& collider) {
    auto itr = std::find(mColliders.begin(), mColliders.end(), collider);
    if (itr != mColliders.end()) {
        std::iter_swap(itr, mColliders.end() - 1);
        mColliders.pop_back();
    }
}

void Physics::clear() {
    mColliders.clear();
}

void Physics::sweepAndPrune() {
    if (mColliders.empty()) {
        return;
    }

    //min.xが小さい順にソート
    std::sort(mColliders.begin(), mColliders.end(), [&](const CollPtr& a, const CollPtr& b) {
        return a->getAABB().min.x < b->getAABB().min.x;
    });

    for (size_t i = 0; i < mColliders.size(); ++i) {
        const auto& aColl = mColliders[i];
        if (!aColl->getEnable()) {
            continue;
        }
        const auto& a = aColl->getAABB();

        for (size_t j = i + 1; j < mColliders.size(); ++j) {
            const auto& bColl = mColliders[j];
            if (!bColl->getEnable()) {
                continue;
            }
            const auto& b = bColl->getAABB();

            //もしb.min.xが、a.max.xを超えていたら、
            //aと交差する可能性があるbは存在しない
            if (b.min.x > a.max.x) {
                break;
            } else if (Intersect::intersectAABB(a, b)) {
                aColl->addHitCollider(bColl);
                bColl->addHitCollider(aColl);
            }
        }
    }
}
