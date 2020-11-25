#pragma once

#include "../../Mesh/IMesh.h"

class IMap {
public:
    virtual ~IMap() = default;
    //マップ情報を取得する
    virtual const IMesh& getMeshData() const = 0;
};
