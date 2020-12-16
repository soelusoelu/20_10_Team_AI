#ifndef _ASCELL_MANAGER_H_
#define _ASCELL_MANAGER_H_

#include "ASCell.h"
#include <vector>
enum class E_State
{
	NONE, OPEN, CLOSE
};
//キャラクターが所持するマップ情報を内包するクラス
class ASCellManager
{

public:
	ASCellManager();
	ASCellManager(std::vector<ASCell> c,int width,int height,Position startPos, Position endPos);
	~ASCellManager();
	//初期処理
	void Initialize();
	//探索開始地点の設定
	void SetStartPosition(Position pos);
	//探索終了地点の設定
	void SetGoalPosition(Position pos);
	//指定したセルに隣接したセルのうちNONE状態のものをOPENにしコストの設定等を行う
	void OpenNeighCells(ASCell* cell);
	//OPEN状態のセルをチェック。条件に応じてCLOSE状態に変更にしたあと、基準セルの選定を行う
	void CheckOpenedCell();
	//指定したセルのコストをその時のゴールセルと基準セルを基に設定する
	void SetCost(ASCell* cells, ASCellSearch* cSearch);
	//ルートのデータを渡す
	std::vector<Position> GetRoute();
	//指定した座標のセルを返す
	ASCell* GetCell(Position pos);
	void StartSearch();

	Position GetStartPosition();

	Position GetGoalPosition();

	std::vector<E_State> cellStates;
	//Mapから取得したCell情報
	std::vector<ASCell> cells;
	//上記に対応した探索用変数
	std::vector<ASCellSearch> cellSearches;

	std::vector<ASCell*> openedCells;
	std::vector<Position> routes;
private: 
	ASCell* startCell;
	ASCell* goalCell;
	ASCell* currentCell;


private:
	int cellsWidth=22, cellsHeight=22;
};

#endif

