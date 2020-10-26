#pragma once

#include "Collider.h"
#include "../../Collision/Collision.h"
#include "../../Math/Math.h"
#include "../../Mesh/IMesh.h"
#include "../../Mesh/IMeshLoader.h"

class AABBCollider : public Collider {
public:
    AABBCollider(GameObject& gameObject);
    ~AABBCollider();
    virtual void start() override;
    virtual void lateUpdate() override;
    virtual void onEnable(bool value) override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;
    //ボックスの最小と最大点を直接設定する
    void set(const Vector3& min, const Vector3& max);
    //AABBを取得する
    const AABB& getAABB() const;
    //当たり判定を可視化するか
    void setRenderCollision(bool value);

private:
    //AABBを作成する
    void createAABB(const IMesh& mesh);
    //メッシュから最小、最大点を割り出す
    void computeMinMax(Vector3& outMin, Vector3& outMax, const MeshVertices& meshVertices);
    //AABBを更新する
    void updateAABB();
    //当たり判定を可視化する
    void renderCollision();

private:
    AABB mAABB;
    Vector3 mDefaultMin;
    Vector3 mDefaultMax;
    bool mIsRenderCollision;
};
