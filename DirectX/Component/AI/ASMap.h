#pragma once

#include "ASCell.h"
#include <vector>
#include "../Component.h"
#include"../../Math/Vector2.h"
#include"../../Math/Vector3.h"

//マップ側が所持するマップ情報のクラス
class ASMap
{
public:
	ASMap();
	~ASMap();
	void Initialize();
	void JudgeSlope(ASCell* cell);

	ASCell* GetCell(Position pos);
	const std::vector<ASCell>& GetCells();
	 Vector2 CalcCellPos(const ASCell& cell);
	 Vector2 ConvertV3toV2(const Vector3& v3);
private:
	std::vector<ASCell>cells;
	float mapWidth = 200, mapHeight = 200;
	int cellsWidth = 10, cellsHeight = 10;
	std::vector<Vector3> vertices;
};

