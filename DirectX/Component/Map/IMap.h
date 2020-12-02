#pragma once
#include "../AI/ASCell.h"
#include "../AI/ASMap.h"
#include "../../Mesh/IMesh.h"

class IMap {
public:
    virtual ~IMap() = default;
    //マップ情報を取得する
    virtual const IMesh& getMeshData() const = 0;
    //マップのCellを取得
    virtual std::vector<ASCell>* GetCellsInfo()=0;
};
