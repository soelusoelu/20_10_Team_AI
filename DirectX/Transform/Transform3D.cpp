#include "Transform3D.h"
#include "ParentChildRelationship.h"
#include "../Engine/DebugManager/DebugLayer/Inspector/ImGuiWrapper.h"
#include "../GameObject/GameObject.h"
#include "../Imgui/imgui.h"
#include "../Utility/LevelLoader.h"

Transform3D::Transform3D(GameObject* gameObject) :
    mGameObject(gameObject),
    mParentChildRelation(std::make_unique<ParentChildRelationship>(this)),
    mWorldTransform(Matrix4::identity),
    mPosition(Vector3::zero),
    mRotation(Quaternion::identity),
    mPivot(Vector3::zero),
    mScale(Vector3::one) {
}

Transform3D::~Transform3D() = default;

void Transform3D::computeWorldTransform() {
    //親がいる場合は親に任せる
    if (mParentChildRelation->parent()) {
        return;
    }

    //自身のワールド行列を計算する
    computeWorld();
    //子のワールド行列を計算する
    computeChildrenTransform();
}

const Matrix4& Transform3D::getWorldTransform() const {
    return mWorldTransform;
}

void Transform3D::setPosition(const Vector3& pos) {
    mPosition = pos;
}

Vector3 Transform3D::getPosition() const {
    auto parent = mParentChildRelation->parent();
    auto pos = mPosition;
    while (parent) {
        pos += parent->transform().mPosition;
        parent = parent->parent();
    }
    return pos;
}

const Vector3& Transform3D::getLocalPosition() const {
    return mPosition;
}

void Transform3D::translate(const Vector3& translation) {
    mPosition += translation;
}

void Transform3D::translate(float x, float y, float z) {
    mPosition.x += x;
    mPosition.y += y;
    mPosition.z += z;
}

void Transform3D::setRotation(const Quaternion& rot) {
    mRotation = rot;
}

void Transform3D::setRotation(const Vector3& axis, float angle) {
    angle *= 0.5f;
    auto sinAngle = Math::sin(angle);

    mRotation.x = axis.x * sinAngle;
    mRotation.y = axis.y * sinAngle;
    mRotation.z = axis.z * sinAngle;
    mRotation.w = Math::cos(angle);
}

void Transform3D::setRotation(const Vector3& eulers) {
    mRotation.setEuler(eulers);
}

Quaternion Transform3D::getRotation() const {
    auto parent = mParentChildRelation->parent();
    auto rotation = mRotation;
    while (parent) {
        rotation = Quaternion::concatenate(rotation, parent->transform().mRotation);
        parent = parent->parent();
    }
    return rotation;
}

const Quaternion& Transform3D::getLocalRotation() const {
    return mRotation;
}

void Transform3D::rotate(const Vector3& axis, float angle) {
    angle *= 0.5f;
    float sinAngle = Math::sin(angle);

    Quaternion inc(
        axis.x * sinAngle,
        axis.y * sinAngle,
        axis.z * sinAngle,
        Math::cos(angle)
    );
    mRotation = Quaternion::concatenate(mRotation, inc);
}

void Transform3D::rotate(const Vector3& eulers) {
    rotate(Vector3::forward, eulers.z);
    rotate(Vector3::right, eulers.x);
    rotate(Vector3::up, eulers.y);
}

void Transform3D::setPivot(const Vector3& pivot) {
    mPivot = pivot;
}

const Vector3& Transform3D::getPivot() const {
    return mPivot;
}

void Transform3D::setScale(const Vector3& scale) {
    mScale = scale;
}

void Transform3D::setScale(float scale) {
    mScale.x = scale;
    mScale.y = scale;
    mScale.z = scale;
}

Vector3 Transform3D::getScale() const {
    auto parent = mParentChildRelation->parent();
    auto scale = mScale;
    while (parent) {
        scale *= parent->transform().mScale;
        parent = parent->parent();
    }
    return scale;
}

const Vector3& Transform3D::getLocalScale() const {
    return mScale;
}

Vector3 Transform3D::forward() const {
    return Vector3::transform(Vector3::forward, mRotation);
}

Vector3 Transform3D::up() const {
    return Vector3::transform(Vector3::up, mRotation);
}

Vector3 Transform3D::right() const {
    return Vector3::transform(Vector3::right, mRotation);
}

GameObject& Transform3D::gameObject() const {
    return *mGameObject;
}

ParentChildRelationship& Transform3D::getParentChildRelation() const {
    return *mParentChildRelation;
}

void Transform3D::loadProperties(const rapidjson::Value& inObj) {
    //位置、回転、スケールを読み込む
    JsonHelper::getVector3(inObj, "position", &mPosition);
    Vector3 rot;
    if (JsonHelper::getVector3(inObj, "rotation", &rot)) {
        setRotation(rot);
    }
    JsonHelper::getVector3(inObj, "scale", &mScale);
    JsonHelper::getVector3(inObj, "pivot", &mPivot);

    computeWorldTransform();
}

void Transform3D::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const {
    //位置、回転、スケールを書き込む
    JsonHelper::setVector3(alloc, inObj, "position", mPosition);
    JsonHelper::setVector3(alloc, inObj, "rotation", mRotation.euler());
    JsonHelper::setVector3(alloc, inObj, "scale", mScale);
    JsonHelper::setVector3(alloc, inObj, "pivot", mPivot);
}

void Transform3D::drawInspector() {
    ImGuiWrapper::dragVector3("Position", mPosition, 0.01f);

    auto euler = mRotation.euler();
    if (ImGuiWrapper::dragVector3("Rotation", euler, 0.1f)) {
        mRotation.setEuler(euler);
    }

    ImGuiWrapper::dragVector3("Scale", mScale, 0.01f);
}

void Transform3D::computeWorld() {
    mWorldTransform = Matrix4::createTranslation(-mPivot); //ピボットを原点に
    mWorldTransform *= Matrix4::createScale(getLocalScale());
    mWorldTransform *= Matrix4::createFromQuaternion(getLocalRotation());
    mWorldTransform *= Matrix4::createTranslation(getLocalPosition());
}

void Transform3D::multiplyParentWorldTransform() {
    auto parent = mParentChildRelation->parent();
    while (parent) {
        mWorldTransform *= parent->transform().mWorldTransform;
        parent = parent->parent();
    }
}

void Transform3D::computeChildrenTransform() {
    const auto& children = mParentChildRelation->getChildren();
    for (const auto& child : children) {
        auto& childTransform = child->transform();
        //子のワールド行列を計算する
        childTransform.computeWorld();

        //さらに子へ降りていく
        child->transform().computeChildrenTransform();
    }

    //子のワールド行列に親のワールド行列を掛け合わせる
    multiplyParentWorldTransform();
}
