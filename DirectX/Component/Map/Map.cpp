#include "Map.h"
#include "../Mesh/MeshComponent.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectFactory.h"
#include "../AI/ASCell.h"
#include "../AI/ASMap.h"

Map::Map(GameObject& gameObject)
    : Component(gameObject)
    , mGroundMesh(nullptr)
{   
}

Map::~Map() = default;

const IMesh& Map::getMeshData() const {
    return *mGroundMesh->getMesh();
}

const Transform3D& Map::getTransform() const {
    return mGroundMesh->transform();
}

void Map::receiveMapData(const std::string& mapName) {
    auto map = GameObjectCreater::create(mapName);
    mGroundMesh = map->componentManager().getComponent<MeshComponent>();
}

const std::vector<ASCell>& Map::GetCellsInfo()
{
    return asmap.GetCells();
}


