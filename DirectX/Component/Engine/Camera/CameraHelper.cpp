#include "CameraHelper.h"
#include "../../../Collision/Collision.h"

Matrix4 CameraHelper::getViewMatrixTakingSphereInCamera(
    const Sphere& sphere,
    int width,
    int height,
    float fov,
    const Vector3& direction,
    const Vector3& up
) {
    //カメラ位置を決定する
    auto pos = getCameraPositionTakingSphereInCamera(sphere, width, height, fov, direction);

    // ビュー行列作成
    return Matrix4::createLookAt(pos, sphere.center, up);
}

Vector3 CameraHelper::getCameraPositionTakingSphereInCamera(
    const Sphere& sphere,
    int width,
    int height,
    float fov,
    const Vector3& direction
) {
    float aspectYDivX = static_cast<float>(height) / static_cast<float>(width);

    //fovYとfovXの小さい方をθとして選択
    float theta = (aspectYDivX >= 1.f) ? fov * aspectYDivX : fov;

    //カメラを引く距離を算出
    float d = sphere.radius / Math::sin(theta / 2.f);

    //カメラ位置を決定する
    return sphere.center - direction * d;
}
