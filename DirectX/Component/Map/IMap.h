#pragma once

#include "../../Mesh/IMesh.h"

class Transform3D;

class IMap {
public:
    virtual ~IMap() = default;
    //マップ情報を取得する
    virtual const IMesh& getMeshData() const = 0;
    //マップのトランスフォームを取得する
    virtual const Transform3D& getTransform() const = 0;
};
