#pragma once
#include "../AI/ASCell.h"
#include "../../../Mesh/IMesh.h"
#include <vector>;

class Transform3D;

class IMap {
public:
    virtual ~IMap() = default;
    //マップ情報を取得する
    virtual const IMesh& getMeshData() const = 0;
    //マップのCellを取得
    virtual const std::vector<ASCell>& GetCellsInfo() const =0;
    //マップのトランスフォームを取得する
    virtual const Transform3D& getTransform() const = 0;
};
