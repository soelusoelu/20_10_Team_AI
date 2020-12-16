#include "Map.h"
#include "../Mesh/MeshComponent.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectFactory.h"
#include "../AI/ASCell.h"
#include "../AI/ASMap.h"
#include "../../Math/Math.h"
#include "../../Transform/Transform3D.h"


Map::Map(GameObject& gameObject)
    : Component(gameObject)
    , mGroundMesh(nullptr)
{   
	//MapのCell生成

	cells.resize(cellsWidth * cellsHeight);
	for (int i = 0; i < cellsWidth * cellsHeight; i++)
	{
		cells[i].position.x = i % cellsWidth;
		cells[i].position.y = (i - cells[i].position.x) / cellsHeight;
		cells[i].posNum = i;
	}
	for (int i = 0; i < cellsWidth * cellsHeight; i++)
	{
		int x = cells[i].position.x;
		int y = cells[i].position.y;
		for (int ix = fmaxf(x - 1, 0); ix < fminf(x + 2, cellsWidth); ix++)
		{
			for (int iy = fmaxf(y - 1, 0); iy < fminf(y + 2, cellsHeight); iy++)
			{
				if (ix != x || iy != y)
				{
					//Cell座標(ix,iy)のCellを隣接Cellとして追加する
					cells[i].neighCells.push_back(&cells[iy * cellsWidth + ix]);
				}
			}
		}
	}
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

	//Mapのポリゴンの範囲を見て通行不可能なポイントの判定をする（高さのみの判定）
	int mpCount = mGroundMesh->getMesh()->getPolygonCount(0);
	const float cellSizeW = mapWidth / cellsWidth;
	const float cellSizeH = mapHeight / cellsHeight;
	Matrix4 mv = mGroundMesh->transform().getWorldTransform();
	for (int i = 0; i < mpCount; i++)
	{
		Triangle mp = mGroundMesh->getMesh()->getPolygon(0, i,mv);
		float pHeight = mp.p0.y + mp.p1.y + mp.p2.y;
		if (pHeight > 0) {
			float xMax = fmaxf(mp.p0.x, fmaxf(mp.p1.x, mp.p2.x));
			float xMin = fminf(mp.p0.x, fminf(mp.p1.x, mp.p2.x));
			float zMax = fmaxf(mp.p0.z, fmaxf(mp.p1.z, mp.p2.z));
			float zMin = fminf(mp.p0.z, fminf(mp.p1.z, mp.p2.z));
			for (int j = 0; j < cells.size(); j++)
			{
				float cx = cells[j].position.x * cellSizeW - (mapWidth / 2);
				float cy = cells[j].position.y * cellSizeH - (mapHeight / 2);

				if (xMin < cx + cellSizeW && cx < xMax
					&& zMin  < cy + cellSizeH && cy < zMax)
				{
					cells[j].canMove = false;
				}
			}
		}

	}
}

const std::vector<ASCell>& Map::GetCellsInfo()const
{
    return cells;
}


