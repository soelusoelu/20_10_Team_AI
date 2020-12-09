#include "ASMap.h"
#include "ASCell.h"
#include <Math.h>
ASMap::ASMap()
{
	Initialize();
}

ASMap::~ASMap()
{
}



void ASMap::Initialize()

{
	cells.resize(cellsWidth * cellsHeight);
	for (int i = 0; i < cellsWidth * cellsHeight; i++)
	{
		cells[i].position.x = i % cellsWidth;
		cells[i].position.y = (i - cells[i].position.x) / cellsWidth;
	}
	for (int i = 0; i < cellsWidth * cellsHeight; i++)
	{
		int x = cells[i].position.x;
		int y = cells[i].position.y;
		//cells[i].neighCells.resize(8);
		int count = 0;
		for (int j = fmaxf(x - 1, 0); j < fminf(x + 2, cellsWidth); j++)
		{
			for (int k = fmaxf(y - 1, 0); k < fminf(y + 2, cellsHeight); k++)
			{
				if (j != x || k != y)
				{
					Position nPos;
					nPos.x = j;
					nPos.y = k;
					cells[i].neighCells.push_back(GetCell(nPos));
					count++;
				}
			}
		}
		cells[i].neighCells.resize(count);
	}

	


}

void ASMap::JudgeSlope(ASCell* cell)
{
	Vector2 cellv1 = CalcCellPos(*cell);
	Vector2 cellv2 = cellv1 + Vector2(mapWidth / cellsWidth, mapHeight / cellsHeight);
	std::vector<Vector3> vertInCell;


}


ASCell* ASMap::GetCell(Position pos)
{
	return &cells[pos.y * cellsWidth + pos.x];
}

const std::vector<ASCell>& ASMap::GetCells()
{
	return cells;
}

Vector2 ASMap::CalcCellPos(const ASCell& cell)
{
	Vector2 v;
	float cellSize = mapWidth / cellsWidth;
	v.x = ((float)cell.position.x / (float)cellsWidth * mapWidth) - (mapWidth / 2) ;
	cellSize = mapHeight / cellsHeight;
	v.y = ((float)cell.position.y / (float)cellsHeight * mapHeight) - (mapHeight / 2);
	return v;
}

Vector2 ASMap::ConvertV3toV2(const Vector3& v3)
{
	Vector2 v2;
	v2.x = v3.x;
	v2.y = v3.z;
	return v2;
}
