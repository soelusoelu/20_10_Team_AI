#pragma once
#include <vector>
#include"../../../Math/Vector2.h"

struct Position {
	int x=0; int y=0;
	bool operator==(const Position p)const
	{
		if (x == p.x && y == p.y)return true;
		else return false;
	}
};
struct ASCell
{
	//マップ情報（マップ生成後の書き換えが不要なクラス
	Position position;
	int posNum;//リスト内で何番目か
	int cost = 1;//そのCell固有のコスト
	bool canMove = true;//移動可能か不可能か
	std::vector<ASCell*> neighCells;//隣接するCell
};

struct ASCellSearch
{
	//探索用の情報（書き換えが必要な変数
	//今後Cell外に移す
	int scoreE = 0, scoreR = 0, score = 0;
	ASCell* parent = nullptr;
};

