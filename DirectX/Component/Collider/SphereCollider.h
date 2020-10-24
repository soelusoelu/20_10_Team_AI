#pragma once

#include "Collider.h"
#include "../../Collision/Collision.h"
#include "../../Math/Math.h"
#include "../../Mesh/IMeshLoader.h"

class SphereCollider : public Collider {
public:
    SphereCollider(GameObject& gameObject);
    ~SphereCollider();
    virtual void start() override;
    virtual void onUpdateWorldTransform() override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;
    //中心位置と半径を直接設定する
    void set(const Vector3& center, float radius);
    //球を取得する
    const Sphere& getSphere() const;

private:
    //メッシュから中心位置を割り出す
    void computeCenter(const std::vector<MeshVertices>& meshesVertices);
    //メッシュから半径を割り出す
    void computeRadius(const std::vector<MeshVertices>& meshesVertices);

private:
    Sphere mSphere;
    Vector3 mDefaultCenter;
    float mDefaultRadius;
};
