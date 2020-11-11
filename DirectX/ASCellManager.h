#ifndef _ASCELL_MANAGER_H_
#define _ASCELL_MANAGER_H_

#include "ASCell.h"
#include <vector>
class ASCellManager
{
public:
	ASCellManager();
	~ASCellManager();
	//初期処理
	void Initialize();
	//探索開始地点の設定
	void SetStartPosition(Position pos);
	//探索終了地点の設定
	void SetGoalPosition(Position pos);
	//指定したセルに隣接したセルのうちNONE状態のものをOPENにしコストの設定等を行う
	void OpenNeighCells(ASCell* cells);
	//OPEN状態のセルをチェック。条件に応じてCLOSE状態に変更にしたあと、基準セルの選定を行う
	void CheckOpenedCell();
	//指定したセルのコストをその時のゴールセルと基準セルを基に設定する
	void SetCost(ASCell* cells);
	//指定した座標のセルを返す
	ASCell* GetCell(Position pos);
	void StartSearch();
	std::vector<ASCell> cells;
	std::vector<ASCell*> openedCells;
	std::vector<Position> routes;
private: 
	ASCell* startCell;
	ASCell* goalCell;
	ASCell* currentCell;


private:
	int cellsWidth=10, cellsHeight=10;
};

#endif

