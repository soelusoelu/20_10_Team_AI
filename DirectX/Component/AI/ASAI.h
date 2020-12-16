#pragma once
#include <vector>
#include "ASCell.h"
#include "ASCellManager.h"
#include "../Component.h"
#include"../../Math/Vector3.h"
#include "../Character/ICharacterManager.h"
class CharacterCommonComponents;


class ASAI :public Component
{
public:
	ASAI(GameObject& obj);
	~ASAI();
	void Initialize();
	virtual void start()override;
	void originalUpdate();

	//Å‹ß‚Ì“G‚Ìî•ñ‚ğæ“¾‚·‚é
	Vector3 GetNearEnemy();

	//float JudgeCharacter(CharacterPtr chara, float distance);


private:
	Vector3 CalcPosition(int phase);

	Position VectorToPosition(const Vector3& v);

	std::unique_ptr<ASCellManager> cellManager;
	std::unique_ptr < ASCellManager> target;
	std::vector<Position> routes;
	int routePhase=0;
	float mapWidth = 110;
	float mapHeight = 110;
	int cellCountW= 22;
	int cellCountH = 22;
	Vector3 routePoint;
	int startX=0, startY=0;
	int goalX = 0, goalY = 0;
	//ICharacterManager manager;
	const ICharacterManager* manager;
	std::vector<ASCell> cells;
	//Position start;
	Position goal;
	Vector3* targetPos;
	//std::vector<ASCell>&cells;
};

