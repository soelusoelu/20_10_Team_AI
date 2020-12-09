#pragma once

#include "IMap.h"
#include "../Component.h"
#include "../AI/ASCell.h"
#include "../AI/ASMap.h"
#include <memory>
#include <string>

class MeshComponent;

class Map : public Component, public IMap {
public:
    Map(GameObject& gameObject);
    ~Map();
    virtual const IMesh& getMeshData() const override;
    virtual const Transform3D& getTransform() const override;

    //外部からデータを受け取る
    void receiveMapData(const std::string& mapName);
    //Cell情報を渡す
    virtual const std::vector<ASCell>& GetCellsInfo() override;


private:
    Map(const Map&) = delete;
    Map& operator=(const Map&) = delete;

private:
    std::shared_ptr<MeshComponent> mGroundMesh;
    ASMap asmap;
};
