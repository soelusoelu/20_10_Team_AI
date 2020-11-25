#pragma once
#include <vector>
#include"../../Math/Vector2.h"

struct Position {
	int x=0; int y=0;
};
struct ASCell
{
	Position position;
	int posNum;
	int cost=1, scoreE=0,scoreR=0,score=0;
	bool canMove = true;
	std::vector<ASCell*> neighCells;
	ASCell* parent = nullptr;
};

