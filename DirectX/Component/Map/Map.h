#pragma once

#include "IMap.h"
#include "../Component.h"
#include <memory>
#include <string>

class MeshComponent;

class Map : public Component, public IMap {
public:
    Map(GameObject& gameObject);
    ~Map();
    virtual const IMesh& getMeshData() const override;
    //外部からデータを受け取る
    void receiveMapData(const std::string& mapName);

private:
    Map(const Map&) = delete;
    Map& operator=(const Map&) = delete;

private:
    std::shared_ptr<MeshComponent> mGroundMesh;
};
