#pragma once

#include "../../../Math/Math.h"

struct Sphere;

class CameraHelper {
public:
    //スフィアがカメラに収まるビュー行列を作成する
    static Matrix4 getViewMatrixTakingSphereInCamera(
        const Sphere& sphere,
        int width,
        int height,
        float fov,
        const Vector3& direction,
        const Vector3& up
    );

    //スフィアが収まるカメラ位置を取得する
    static Vector3 getCameraPositionTakingSphereInCamera(
        const Sphere& sphere,
        int width,
        int height,
        float fov,
        const Vector3& direction
    );

private:
    CameraHelper() = delete;
    ~CameraHelper() = delete;
};
