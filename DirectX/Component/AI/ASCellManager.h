#ifndef _ASCELL_MANAGER_H_
#define _ASCELL_MANAGER_H_

#include "ASCell.h"
#include <vector>
class ASCellManager
{
public:
	ASCellManager();
	ASCellManager(int width,int height,Position startPos, Position endPos);
	~ASCellManager();
	//��������
	void Initialize();
	//�T���J�n�n�_�̐ݒ�
	void SetStartPosition(Position pos);
	//�T���I���n�_�̐ݒ�
	void SetGoalPosition(Position pos);
	//�w�肵���Z���ɗאڂ����Z���̂���NONE��Ԃ̂��̂�OPEN�ɂ��R�X�g�̐ݒ蓙���s��
	void OpenNeighCells(ASCell* cells);
	//OPEN��Ԃ̃Z�����`�F�b�N�B�����ɉ�����CLOSE��ԂɕύX�ɂ������ƁA��Z���̑I����s��
	void CheckOpenedCell();
	//�w�肵���Z���̃R�X�g�����̎��̃S�[���Z���Ɗ�Z������ɐݒ肷��
	void SetCost(ASCell* cells);
	//���[�g�̃f�[�^��n��
	std::vector<Position> GetRoute();
	//�w�肵�����W�̃Z����Ԃ�
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
