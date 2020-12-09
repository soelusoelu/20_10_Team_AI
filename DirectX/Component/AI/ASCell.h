#pragma once
#include <vector>
#include"../../Math/Vector2.h"

struct Position {
	int x=0; int y=0;
};
struct ASCell
{
	//マップ情報（マップ生成後の書き換えが不要なクラス
	Position position;
	int posNum;
	int cost = 1;
	bool canMove = true;
	std::vector<ASCell*> neighCells;

	//探索用の情報（書き換えが必要な変数
	//今後Cell外に移す
	int scoreE = 0, scoreR = 0, score = 0;
	ASCell* parent = nullptr;
};

