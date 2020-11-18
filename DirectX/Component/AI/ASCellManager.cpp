#include "ASCellManager.h"
#include "ASCell.h"
#include <Math.h>


ASCellManager::ASCellManager()
{
	Initialize();
	//�X�^�[�g�n�_�ƃS�[���n�_���u��
	Position pos;
	pos.x = 0;
	pos.y = 0;
	SetStartPosition(pos);
	pos.x = cellsWidth-1;
	pos.y = cellsHeight-1;
	SetGoalPosition(pos);
	currentCell = startCell;
	//�T���J�n
	//StartSearch();

}

ASCellManager::ASCellManager(int width, int height, Position startPos, Position endPos)
{
	cellsWidth = width;
	cellsHeight = height;
	Initialize();
	//�X�^�[�g�n�_�ƃS�[���n�_���u��
	Position pos=startPos;
	SetStartPosition(pos);
	pos = endPos;
	SetGoalPosition(pos);
	currentCell = startCell;
	//�T���J�n
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
	}
	for (int i = 0; i < cellsWidth * cellsHeight; i++)
	{
		int x = cells[i].position.x;
		int y = cells[i].position.y;
		//cells[i].neighCells.resize(8);
		int count = 0;
		for (int j = fmax(x - 1, 0); j < fmin(x + 2, cellsWidth); j++)
		{
			for (int k = fmax(y - 1, 0); k < fmin(y + 2, cellsHeight); k++)
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
}



void ASCellManager::SetStartPosition(Position pos)
{
	startCell= GetCell(pos);
}

//�אڂ���Z�����I�[�v����Ԃɂ��A�ċA���Čo�H���肷�邽�߂Ɏ������g��e�Ƃ��ēo�^����
void ASCellManager::OpenNeighCells(ASCell* cell)
{
	for (int i = 0; i < cell->neighCells.size(); i++)
	{
		if (cell->neighCells[i]->state == NONE)
		{
			cell->neighCells[i]->state = OPEN;
			cell->neighCells[i]->parent = cell;
			SetCost(cell->neighCells[i]);
			openedCells.push_back(cell->neighCells[i]);
		}
	}
	cell->state = CLOSE;
}

void ASCellManager::CheckOpenedCell()
{
	//CLOSE��Ԃ̂��̂����݂��Ȃ����`�F�b�N
	for (int i=0;i<openedCells.size();i++)
	{
		int count = 0;
		int neighCount = openedCells[i]->neighCells.size();
		for (int j = 0; j < neighCount; j++)
		{
			if (openedCells[i]->neighCells[j]->state != NONE)count++;
		}
		if ((openedCells[i]->state == CLOSE||count==neighCount)&& openedCells[i] != goalCell)
		{
			openedCells[i]->state == CLOSE;
			openedCells.erase(openedCells.begin() + i);
			i--;
		}
	}
	//�X�R�A���ł��Ⴂ���̂���X�R�A�ɂ���B
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
	int x = fabs(cells->position.x- goalCell->position.x);
	int y = fabs(cells->position.y - goalCell->position.y);
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