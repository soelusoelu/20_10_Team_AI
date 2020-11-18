#pragma once
#include <vector>
#include "ASCell.h"
#include "ASCellManager.h"
#include "../Component.h"
#include"../../Math/Vector3.h"

class ASAI :public Component
{
public:
	ASAI(GameObject& obj);
	~ASAI();
	void Initialize();
	virtual void start()override;
	virtual void update()override;

private:
	Vector3 CalcPosition(int phase);

	ASCellManager cellManager;
	std::vector<Position> routes;
	int routePhase=0;
	float mapWidth = 200;
	float mapHeight = 200;
	int cellCountW=10;
	int cellCountH=10;
	Vector3 routePoint;

};

