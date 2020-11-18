#include "ASAI.h"
#include "../../Transform/Transform3D.h"
#include"../../Math/Vector3.h"
#include"../../Utility/Random.h"

ASAI::ASAI(GameObject& obj):Component(obj)
{
}

ASAI::~ASAI()
{
}

void ASAI::Initialize()
{
}

void ASAI::start()
{
	Position start;
	start.x = 0;
	start.y = 0;
	Position goal;
	goal.x = 9;
	goal.y = 9;
	cellManager = ASCellManager(cellCountW, cellCountH, start, goal);
	routes = cellManager.GetRoute(); 
	routePhase = 0;
	routePoint=CalcPosition(routePhase);
	routePhase++;
	transform().setPosition(Vector3(-90, 0, -90));
}

void ASAI::update()
{
	Vector3 v3=routePoint-transform().getPosition();
	float distance = v3.length();
	v3.normalize();
	transform().translate(v3/3);
	if (distance < 1 && routePhase < routes.size())
	{
		routePoint = CalcPosition(routePhase);
		routePhase++;
	}
}

Vector3 ASAI::CalcPosition(int phase)
{
	Vector3 v;
	float cellSize = mapWidth / cellCountW;
	v.x = ((float)routes[phase].x/ (float)cellCountW*mapWidth)-(mapWidth/2)+(cellSize/2);
	cellSize = mapHeight / cellCountH;
	v.z = ((float)routes[phase].y / (float)cellCountH * mapHeight) - (mapHeight / 2) + (cellSize / 2);
	v.y = transform().getPosition().y;
	return v;
}
