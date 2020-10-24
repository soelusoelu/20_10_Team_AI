#include "AABBCollider.h"
#include "../Mesh/MeshComponent.h"
#include "../../DebugLayer/Debug.h"
#include "../../Mesh/IMesh.h"
#include "../../Transform/Transform3D.h"
#include <array>

AABBCollider::AABBCollider(GameObject& gameObject) :
    Collider(gameObject),
    mAABB(),
    mDefaultMin(Vector3::zero),
    mDefaultMax(Vector3::one) {
}

AABBCollider::~AABBCollider() = default;

void AABBCollider::start() {
    Collider::start();

    auto meshComponent = getComponent<MeshComponent>();
    if (meshComponent) {
        const auto& mesh = meshComponent->getMesh();
        const auto& meshesVertices = mesh.getMeshesVertices();
        computeBox(meshesVertices);
        mAABB.min = mDefaultMin;
        mAABB.max = mDefaultMax;

        //早速transformが変わっているかもしれないから更新する
        updateAABB();
    }
}

void AABBCollider::update() {
#ifdef _DEBUG
    //デバッグ時のみ当たり判定を表示
    std::array<Vector3, 8> points;
    const auto& min = mAABB.min;
    const auto& max = mAABB.max;
    points[0] = min;
    points[1] = Vector3(max.x, min.y, min.z);
    points[2] = Vector3(min.x, min.y, max.z);
    points[3] = Vector3(max.x, min.y, max.z);
    points[4] = Vector3(min.x, max.y, min.z);
    points[5] = Vector3(max.x, max.y, min.z);
    points[6] = Vector3(min.x, max.y, max.z);
    points[7] = max;

    Debug::renderLine(points[0], points[1], ColorPalette::lightGreen);
    Debug::renderLine(points[0], points[2], ColorPalette::lightGreen);
    Debug::renderLine(points[2], points[3], ColorPalette::lightGreen);
    Debug::renderLine(points[1], points[3], ColorPalette::lightGreen);

    Debug::renderLine(points[4], points[5], ColorPalette::lightGreen);
    Debug::renderLine(points[4], points[6], ColorPalette::lightGreen);
    Debug::renderLine(points[6], points[7], ColorPalette::lightGreen);
    Debug::renderLine(points[5], points[7], ColorPalette::lightGreen);

    Debug::renderLine(points[0], points[4], ColorPalette::lightGreen);
    Debug::renderLine(points[1], points[5], ColorPalette::lightGreen);
    Debug::renderLine(points[2], points[6], ColorPalette::lightGreen);
    Debug::renderLine(points[3], points[7], ColorPalette::lightGreen);
#endif // _DEBUG
}

void AABBCollider::onUpdateWorldTransform() {
    Collider::onUpdateWorldTransform();

    if (!mIsAutoUpdate) {
        return;
    }

    updateAABB();
}

void AABBCollider::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    Collider::drawDebugInfo(inspect);

    inspect->emplace_back("Min", mAABB.min);
    inspect->emplace_back("Max", mAABB.max);
}

void AABBCollider::set(const Vector3& min, const Vector3& max) {
    mAABB.min = min;
    mAABB.max = max;
    if (mIsAutoUpdate) {
        mIsAutoUpdate = false;
    }
}

const AABB& AABBCollider::getAABB() const {
    return mAABB;
}

void AABBCollider::computeBox(const std::vector<MeshVertices>& meshesVertices) {
    auto min = Vector3::one * Math::infinity;
    auto max = Vector3::one * Math::negInfinity;

    //すべてのメッシュ情報から最小、最大点を割り出す
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

    mDefaultMin = min;
    mDefaultMax = max;
}

void AABBCollider::updateAABB() {
    const auto& t = transform();
    const auto& pos = t.getPosition();
    const auto& scale = t.getScale();
    //回転を適用するために一時的なAABBを作成する
    AABB aabb(mDefaultMin - t.getPivot(), mDefaultMax - t.getPivot());
    //回転を適用する
    aabb.rotate(t.getRotation());
    //既存のAABBを更新する
    mAABB.min = aabb.min * scale + pos;
    mAABB.max = aabb.max * scale + pos;
}
