#pragma once

#include "Collider.h"
#include "../../Collision/Collision.h"
#include "../../Math/Math.h"
#include "../../Mesh/IMeshLoader.h"

class AABBCollider : public Collider {
public:
    AABBCollider(GameObject& gameObject);
    ~AABBCollider();
    virtual void start() override;
    virtual void update() override;
    virtual void onUpdateWorldTransform() override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;
    //ボックスの最小と最大点を直接設定する
    void set(const Vector3& min, const Vector3& max);
    //AABBを取得する
    const AABB& getAABB() const;

private:
    //メッシュから最小、最大点を割り出す
    void computeBox(const std::vector<MeshVertices>& meshesVertices);
    //AABBを更新する
    void updateAABB();

private:
    AABB mAABB;
    Vector3 mDefaultMin;
    Vector3 mDefaultMax;
};
