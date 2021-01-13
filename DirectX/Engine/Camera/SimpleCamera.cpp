#include "SimpleCamera.h"
#include "../../Collision/Collision.h"
#include "../../System/Window.h"

SimpleCamera::SimpleCamera()
    : mView()
    , mProjection()
    , mPosition()
    , mLookAt(Vector3::forward * 10.f)
    , mUp(Vector3::up)
    , mFOV(45.f)
    , mNearClip(0.1f)
    , mFarClip(1000.f)
{
    calcView();
    calcProjection();
}

SimpleCamera::~SimpleCamera() = default;

void SimpleCamera::setView(const Matrix4& view) {
    mView = view;
}

const Matrix4& SimpleCamera::getView() const {
    return mView;
}

void SimpleCamera::setProjection(const Matrix4& projection) {
    mProjection = projection;
}

const Matrix4& SimpleCamera::getProjection() const {
    return mProjection;
}

Matrix4 SimpleCamera::getViewProjection() const {
    return mView * mProjection;
}

void SimpleCamera::setPosition(const Vector3& pos) {
    mPosition = pos;
    calcView();
}

Vector3 SimpleCamera::getPosition() const {
    return mPosition;
}

void SimpleCamera::lookAt(const Vector3& position) {
    mLookAt = position;
    calcView();
}

const Vector3& SimpleCamera::getLookAt() const {
    return mLookAt;
}

void SimpleCamera::setFov(float fov) {
    mFOV = fov;
    calcProjection();
}

float SimpleCamera::getFov() const {
    return mFOV;
}

void SimpleCamera::setNearClip(float nearClip) {
    mNearClip = nearClip;
    calcProjection();
}

float SimpleCamera::getNearClip() const {
    return mNearClip;
}

void SimpleCamera::setFarClip(float farClip) {
    mFarClip = farClip;
    calcProjection();
}

float SimpleCamera::getFarClip() const {
    return mFarClip;
}

Vector3 SimpleCamera::screenToWorldPoint(const Vector2& position, float z) const {
    //ビューポート、射影、ビュー、それぞれの逆行列を求める
    auto invView = Matrix4::inverse(mView);
    auto invProj = Matrix4::inverse(mProjection);

    auto invViewport = Matrix4::identity;
    invViewport.m[0][0] = Window::width() / 2.f;
    invViewport.m[1][1] = -Window::height() / 2.f;
    invViewport.m[3][0] = Window::width() / 2.f;
    invViewport.m[3][1] = Window::height() / 2.f;
    invViewport.inverse();

    //ビューポート、射影、ビュー、それぞれの逆行列を掛ける
    auto m = invViewport * invProj * invView;

    //スクリーン座標をワールド座標に変換
    return Vector3::transformWithPerspDiv(Vector3(position, z), m);
}

Ray SimpleCamera::screenToRay(const Vector2& position, float z) const {
    Ray ray;
    ray.start = mPosition;
    ray.end = screenToWorldPoint(position, z);

    return ray;
}

void SimpleCamera::calcView() {
    mView = Matrix4::createLookAt(mPosition, mLookAt, mUp);
}

void SimpleCamera::calcProjection() {
    mProjection = Matrix4::createPerspectiveFOV(Window::width(), Window::height(), mFOV, mNearClip, mFarClip);
}
