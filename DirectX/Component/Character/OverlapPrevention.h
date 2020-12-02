#pragma once

#include "../Component.h"
#include "../../Collision/Collision.h"
#include "../../Math/Math.h"

class CharacterCommonComponents;
class AABBCollider;

//キャラの重なりを防止するクラス
class OverlapPrevention : public Component {
    using CharacterPtr = std::shared_ptr<CharacterCommonComponents>;
    using CharacterPtrList = std::list<CharacterPtr>;

public:
    OverlapPrevention(GameObject& gameObject);
    ~OverlapPrevention();
    //ターゲットを押し出し、重なりを防止する
    void overlapPrevent(
        const AABBCollider& target,
        const CharacterPtrList& characters,
        const Vector3& movePos,
        const Vector3& previousPos
    ) const;

private:
    //押し出し処理
    void extrudeX(const AABBCollider& target, const AABBCollider& other, const Vector3& movePos, float moveAmountX) const;
    void extrudeY(const AABBCollider& target, const AABBCollider& other, const Vector3& movePos, float moveAmountY) const;
    void extrudeZ(const AABBCollider& target, const AABBCollider& other, const Vector3& movePos, float moveAmountZ) const;

private:
    OverlapPrevention(const OverlapPrevention&) = delete;
    OverlapPrevention& operator=(const OverlapPrevention&) = delete;
};
