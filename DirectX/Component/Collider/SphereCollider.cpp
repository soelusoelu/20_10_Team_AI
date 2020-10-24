#include "SphereCollider.h"
#include "../Mesh/MeshComponent.h"
#include "../../Mesh/IMesh.h"
#include "../../Transform/Transform3D.h"

SphereCollider::SphereCollider(GameObject& gameObject) :
    Collider(gameObject),
    mSphere(),
    mDefaultCenter(Vector3::zero),
    mDefaultRadius(0.f) {
}

SphereCollider::~SphereCollider() = default;

void SphereCollider::start() {
    Collider::start();

    auto meshComponent = getComponent<MeshComponent>();
    if (meshComponent) {
        const auto& mesh = meshComponent->getMesh();
        const auto& meshesVertices = mesh.getMeshesVertices();
        computeCenter(meshesVertices);
        computeRadius(meshesVertices);
        mSphere.center = mDefaultCenter;
        mSphere.radius = mDefaultRadius;
    }
}

void SphereCollider::onUpdateWorldTransform() {
    Collider::onUpdateWorldTransform();

    if (!mIsAutoUpdate) {
        return;
    }

    auto center = mDefaultCenter + transform().getPosition();
    auto scale = transform().getScale();
    auto maxScaleValue = Math::Max<float>(scale.x, Math::Max<float>(scale.y, scale.z));
    auto radius = mDefaultRadius * maxScaleValue;

    mSphere.center = center;
    mSphere.radius = radius;
}

void SphereCollider::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    Collider::drawDebugInfo(inspect);

    inspect->emplace_back("Center", mSphere.center);
    inspect->emplace_back("Radius", mSphere.radius);
}

void SphereCollider::set(const Vector3& center, float radius) {
    mSphere.center = center;
    mSphere.radius = radius;
    if (mIsAutoUpdate) {
        mIsAutoUpdate = false;
    }
}

const Sphere& SphereCollider::getSphere() const {
    return mSphere;
}

void SphereCollider::computeCenter(const std::vector<MeshVertices>& meshesVertices) {
    auto min = Vector3::one * Math::infinity;
    auto max = Vector3::one * Math::negInfinity;

    //すべてのメッシュ情報から中心位置を割り出す
    for (size_t i = 0; i < meshesVertices.size(); ++i) {
        const auto& vertices = meshesVertices[i];
        for (size_t j = 0; j < vertices.size(); ++j) {
            const auto& p = vertices[j].pos;
            if (p.x < min.x) {
                min.x = p.x;
            }
            if (p.x > max.x) {
                max.x = p.x;
            }
            if (p.y < min.y) {
                min.y = p.y;
            }
            if (p.y > max.y) {
                max.y = p.y;
            }
            if (p.z < min.z) {
                min.z = p.z;
            }
            if (p.z > max.z) {
                max.z = p.z;
            }
        }
    }

    mDefaultCenter = (max + min) / 2.f;
}

void SphereCollider::computeRadius(const std::vector<MeshVertices>& meshesVertices) {
    float min = Math::infinity;
    float max = Math::negInfinity;

    //すべてのメッシュ情報から半径を割り出す
    for (size_t i = 0; i < meshesVertices.size(); ++i) {
        const auto& vertices = meshesVertices[i];
        for (size_t j = 0; j < vertices.size(); ++j) {
            const auto& p = vertices[j].pos;
            if (p.x < min) {
                min = p.x;
            }
            if (p.x > max) {
                max = p.x;
            }
            if (p.y < min) {
                min = p.y;
            }
            if (p.y > max) {
                max = p.y;
            }
            if (p.z < min) {
                min = p.z;
            }
            if (p.z > max) {
                max = p.z;
            }
        }
    }
    mDefaultRadius = (max - min) / 2.f;
}
