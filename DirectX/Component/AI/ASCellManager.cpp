#include "ASCellManager.h"
#include "ASCell.h"
#include <Math.h>


ASCellManager::ASCellManager()
{
	Initialize();
	//スタート地点とゴール地点仮置き
	Position pos;
	pos.x = 0;
	pos.y = 0;
	SetStartPosition(pos);
	pos.x = cellsWidth-1;
	pos.y = cellsHeight-1;
	SetGoalPosition(pos);
	currentCell = startCell;
	//探索開始
	//StartSearch();

}

ASCellManager::ASCellManager(int width, int height, Position startPos, Position endPos)
{	
	cellsWidth = width;
	cellsHeight = height;
	Initialize();
	//スタート地点とゴール地点仮置き
	Position pos=startPos;
	SetStartPosition(pos);
	pos = endPos;
	SetGoalPosition(pos);
	currentCell = startCell;
	//探索開始
	StartSearch();
}

ASCellManager::~ASCellManager()
{

}

void ASCellManager::Initialize()
{
	cells.resize(cellsWidth * cellsHeight);
	for (int i = 0; i < cellsWidth * cellsHeight; i++)
	{
		cells[i].position.x = i % cellsWidth;
		cells[i].position.y = (i - cells[i].position.x) / cellsWidth;
		cells[i].posNum = i;
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
					//cells[i].neighCells[count] = GetCell(nPos);
					cells[i].neighCells.push_back(GetCell(nPos));
					count++;
				}
			}
		}
		cells[i].neighCells.resize(count);
	}
	cellStates.resize(cells.size());
	for (int i = 0; i < cellStates.size(); i++)
	{
		cellStates[i] = E_State::NONE;
	}
}



void ASCellManager::SetStartPosition(Position pos)
{
	startCell= GetCell(pos);
}

//隣接するセルをオープン状態にし、再帰して経路決定するために自分自身を親として登録する
void ASCellManager::OpenNeighCells(ASCell* cell)
{
	for (int i = 0; i < cell->neighCells.size(); i++)
	{
		if (cellStates[cell->neighCells[i]->posNum] == E_State::NONE&& cell->neighCells[i]->canMove)
		{
			//cell->neighCells[i]->state = E_State::OPEN;
			cellStates[i] = E_State::OPEN;
			cell->neighCells[i]->parent = cell;
			SetCost(cell->neighCells[i]);
			openedCells.push_back(cell->neighCells[i]);
		}
	}
	cellStates[cell->posNum] = E_State::CLOSE;
}

void ASCellManager::CheckOpenedCell()
{
	//CLOSE状態のものが存在しないかチェック
	for (int i=0;i<openedCells.size();i++)
	{
		int count = 0;
		int neighCount = openedCells[i]->neighCells.size();
		for (int j = 0; j < neighCount; j++)
		{
			if (cellStates[openedCells[i]->neighCells[j]->posNum] != E_State::NONE)count++;
		}
		if ((cellStates[openedCells[i]->posNum] == E_State::CLOSE||count==neighCount)&& openedCells[i] != goalCell)
		{
			cellStates[openedCells[i]->posNum] == E_State::CLOSE;
			openedCells.erase(openedCells.begin() + i);
			i--;
		}
	}
	//スコアが最も低いものを基準スコアにする。
	ASCell* minScoreCell= openedCells[0];
	for (int i = 1; i < openedCells.size(); i++)
	{
		if (openedCells[i]->score < minScoreCell->score)
		{
			minScoreCell= openedCells[i];
		}
	}
	currentCell = minScoreCell;
}

void ASCellManager::SetGoalPosition(Position pos)
{
	goalCell = GetCell(pos);
}

void ASCellManager::SetCost(ASCell* cells)
{
	int x = fabsf(cells->position.x- goalCell->position.x);
	int y = fabsf(cells->position.y - goalCell->position.y);
	cells->scoreE = fmax(x,y);
	cells->scoreR = currentCell->scoreR + 1;
	cells->score = cells->scoreE + cells->scoreR;
}

std::vector<Position> ASCellManager::GetRoute()
{
	return routes;
}

ASCell* ASCellManager::GetCell(Position pos)
{
	return &cells[pos.y*cellsWidth+pos.x];
}

void ASCellManager::StartSearch()
{
	while (currentCell != goalCell)
	{
		OpenNeighCells(currentCell);
		CheckOpenedCell();
	}
	while (currentCell != startCell)
	{
		routes.insert(routes.begin(), currentCell->position);
		currentCell = currentCell->parent;
	}
}

Position ASCellManager::GetStartPosition()
{
	return startCell->position;
}

Position ASCellManager::GetGoalPosition()
{
	return goalCell->position;
}
