#pragma once

#include "../../../Math/Math.h"

struct Sphere;

class CameraHelper {
public:
    //スフィアがカメラに収まるビュー行列を作成する
    static Matrix4 getViewMatrixTakingSphereInCamera(
        const Sphere& sphere,
        float aspectYDivX,
        float fov,
        const Vector3& direction,
        const Vector3& up
    );

    //スフィアがカメラに収まるビュー行列を作成する
    static Matrix4 getViewMatrixTakingSphereInCamera(
        const Sphere& sphere,
        int width,
        int height,
        float fov,
        const Vector3& direction,
        const Vector3& up
    );

private:
    CameraHelper() = delete;
    ~CameraHelper() = delete;
};
