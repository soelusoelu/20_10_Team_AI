#include "OverlapPrevention.h"
#include "CharacterCommonComponents.h"
#include "../../Engine/Collider/AABBCollider.h"
#include "../../../Transform/Transform3D.h"

OverlapPrevention::OverlapPrevention(GameObject& gameObject)
    : Component(gameObject)
{
}

OverlapPrevention::~OverlapPrevention() = default;

void OverlapPrevention::overlapPrevent(
    const AABBCollider& target,
    const CharacterPtrList& characters,
    const Vector3& movePos,
    const Vector3& previousPos
) const {
    const auto& targetAabb = target.getAABB();

    for (const auto& other : characters) {
        //同一キャラならスキップ
        if (&target.gameObject() == &other->gameObject()) {
            continue;
        }

        //衝突対象のAABB
        const auto& otherAabbCollider = other->getAABBCollider();
        const auto& otherAabb = otherAabbCollider.getAABB();

        //押し出す
        extrudeX(target, otherAabbCollider, movePos, movePos.x - previousPos.x);
        extrudeY(target, otherAabbCollider, movePos, movePos.y - previousPos.y);
        extrudeZ(target, otherAabbCollider, movePos, movePos.z - previousPos.z);
    }
}

void OverlapPrevention::extrudeX(const AABBCollider& target, const AABBCollider& other, const Vector3& movePos, float moveAmountX) const {
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

    //衝突対象のx位置
    const auto otherPosX = other.transform().getPosition().x;

    //押し出す量
    auto extrudeAmount = otherPosX - otherAabb.min.x;
    extrudeAmount += targetAabb.max.x - movePos.x;
    if (moveAmountX < 0.f) {
        extrudeAmount *= -1.f;
    }

    //押し出す
    target.transform().setPosition(Vector3(
        otherPosX - extrudeAmount,
        movePos.y,
        movePos.z
    ));
}

void OverlapPrevention::extrudeY(const AABBCollider& target, const AABBCollider& other, const Vector3& movePos, float moveAmountY) const {
    //y軸方向に移動していないなら終了
    if (Math::nearZero(moveAmountY)) {
        return;
    }

    //押し出されるターゲットのAABB
    auto targetAabb = target.getAABB();
    //衝突対象のAABB
    const auto& otherAabb = other.getAABB();

    //移動量分当たり判定をずらす
    targetAabb.min.y += moveAmountY;
    targetAabb.max.y += moveAmountY;

    //衝突していないなら終了
    if (!Intersect::intersectAABB(targetAabb, otherAabb)) {
        return;
    }

    //衝突対象のy位置
    const auto otherPosY = other.transform().getPosition().y;

    //押し出す量
    auto extrudeAmount = otherPosY - otherAabb.min.y;
    extrudeAmount += targetAabb.max.y - movePos.y;
    if (moveAmountY < 0.f) {
        extrudeAmount *= -1.f;
    }

    //押し出す
    target.transform().setPosition(Vector3(
        movePos.x,
        otherPosY - extrudeAmount,
        movePos.z
    ));
}

void OverlapPrevention::extrudeZ(const AABBCollider& target, const AABBCollider& other, const Vector3& movePos, float moveAmountZ) const {
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

    //衝突対象のz位置
    const auto otherPosZ = other.transform().getPosition().z;

    //押し出す量
    auto extrudeAmount = otherPosZ - otherAabb.min.z;
    extrudeAmount += targetAabb.max.z - movePos.z;
    if (moveAmountZ < 0.f) {
        extrudeAmount *= -1.f;
    }

    //押し出す
    target.transform().setPosition(Vector3(
        movePos.x,
        movePos.y,
        otherPosZ - extrudeAmount
    ));
}
