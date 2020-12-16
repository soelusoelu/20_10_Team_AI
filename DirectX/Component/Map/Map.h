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
    //Cell情報を渡す
    virtual const std::vector<ASCell>& GetCellsInfo()const override;


    //外部からデータを受け取る
    void receiveMapData(const std::string& mapName);


private:
    Map(const Map&) = delete;
    Map& operator=(const Map&) = delete;

private:
    std::shared_ptr<MeshComponent> mGroundMesh;
    std::vector<ASCell> cells;
    int cellsWidth = 22, cellsHeight = 22;
    float mapWidth = 110;float mapHeight = 110;
private:
    bool cellCanMoveCreated = false;
};
