#pragma once
#include <vector>
#include"../../Math/Vector2.h"
enum class E_State
{
	NONE, OPEN, CLOSE
};
struct Position {
	int x=0; int y=0;
};
struct ASCell
{
	E_State state = E_State::NONE;
	Position position;
	int cost=1, scoreE=0,scoreR=0,score=0;
	bool canMove = true;
	std::vector<ASCell*> neighCells;
	ASCell* parent = nullptr;
};

