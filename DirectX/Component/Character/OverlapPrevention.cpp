#include "OverlapPrevention.h"
#include "CharacterCommonComponents.h"
#include "../Collider/AABBCollider.h"
#include "../../DebugLayer/Debug.h"
#include "../../Transform/Transform3D.h"

OverlapPrevention::OverlapPrevention(GameObject& gameObject)
    : Component(gameObject)
{
}

OverlapPrevention::~OverlapPrevention() = default;

void OverlapPrevention::overlapPrevent(CharacterCommonComponents& target, const CharacterPtrList& characters, const Vector3& movePos, const Vector3& previousPos) const {
    const auto& targetAabbCollider = target.getAABBCollider();
    const auto& targetAabb = targetAabbCollider.getAABB();

    for (const auto& other : characters) {
        //同一キャラならスキップ
        if (&target == other.get()) {
            continue;
        }

        //衝突対象のAABB
        const auto& otherAabbCollider = other->getAABBCollider();
        const auto& otherAabb = otherAabbCollider.getAABB();

        //押し出す
        extrudeX(targetAabbCollider, otherAabbCollider, movePos.x - previousPos.x);
        //extrudeY(targetAabbCollider, otherAabbCollider, movePos.y - previousPos.y);
        extrudeZ(targetAabbCollider, otherAabbCollider, movePos.z - previousPos.z);
    }
}

void OverlapPrevention::extrudeX(const AABBCollider& target, const AABBCollider& other, float moveAmountX) const {
    //x軸方向に移動していないなら終了
    if (Math::nearZero(moveAmountX)) {
        return;
    }

    //押し出されるターゲットのAABB
    auto targetAabb = target.getAABB();
    //衝突対象のAABB
    const auto& otherAabb = other.getAABB();

    //移動量分当たり判定をずらす
    targetAabb.min.x += moveAmountX;
    targetAabb.max.x += moveAmountX;

    //衝突していないなら終了
    if (!Intersect::intersectAABB(targetAabb, otherAabb)) {
        return;
    }

    //押し出されるターゲットのトランスフォーム
    auto& targetT = target.transform();
    //衝突対象のトランスフォーム
    const auto& otherT = other.transform();

    //押し出す量
    auto extrudeAmount = 0.f;
    if (moveAmountX > 0.f) {
        extrudeAmount = otherT.getPosition().x - otherAabb.min.x;
        extrudeAmount += targetAabb.max.x - targetT.getPosition().x;
    } else {
        extrudeAmount = otherAabb.max.x - otherT.getPosition().x;
        extrudeAmount += targetT.getPosition().x - targetAabb.min.x;
        extrudeAmount *= -1.f;
    }

    //押し出す
    const auto& prePos = targetT.getPosition();
    targetT.setPosition(Vector3(
        otherT.getPosition().x - extrudeAmount,
        prePos.y,
        prePos.z
    ));
}

void OverlapPrevention::extrudeY(const AABBCollider& target, const AABBCollider& other, float moveAmountY) const {
}

void OverlapPrevention::extrudeZ(const AABBCollider& target, const AABBCollider& other, float moveAmountZ) const {
    //z軸方向に移動していないなら終了
    if (Math::nearZero(moveAmountZ)) {
        return;
    }

    //押し出されるターゲットのAABB
    auto targetAabb = target.getAABB();
    //衝突対象のAABB
    const auto& otherAabb = other.getAABB();

    //移動量分当たり判定をずらす
    targetAabb.min.z += moveAmountZ;
    targetAabb.max.z += moveAmountZ;

    //衝突していないなら終了
    if (!Intersect::intersectAABB(targetAabb, otherAabb)) {
        return;
    }

    //押し出されるターゲットのトランスフォーム
    auto& targetT = target.transform();
    //衝突対象のトランスフォーム
    const auto& otherT = other.transform();

    //押し出す量
    auto extrudeAmount = 0.f;
    if (moveAmountZ > 0.f) {
        extrudeAmount = otherT.getPosition().z - otherAabb.min.z;
        extrudeAmount += targetAabb.max.z - targetT.getPosition().z;
    } else {
        extrudeAmount = otherAabb.max.z - otherT.getPosition().z;
        extrudeAmount += targetT.getPosition().z - targetAabb.min.z;
        extrudeAmount *= -1.f;
    }

    //押し出す
    const auto& prePos = targetT.getPosition();
    targetT.setPosition(Vector3(
        prePos.x,
        prePos.y,
        otherT.getPosition().z - extrudeAmount
    ));
}
