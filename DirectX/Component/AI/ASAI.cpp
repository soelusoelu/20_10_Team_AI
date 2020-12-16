#include "ASAI.h"
#include "../../Transform/Transform3D.h"
#include"../../Math/Vector3.h"
#include "ASCellManager.h"
#include "../../GameObject/GameObject.h"
#include"../Character/CharacterCommonComponents.h"
#include "../Character/ICharacterManager.h"

ASAI::ASAI(GameObject& obj):Component(obj)
{

}

ASAI::~ASAI()
{
}

void ASAI::Initialize()
{
	
	Position start = VectorToPosition(transform().getPosition());
	start.x = fmaxf(0,fminf( start.x, cellCountW - 1));
	start.y = fmaxf(0, fminf(start.y,cellCountH-1));
	goal = VectorToPosition(GetNearEnemy());
	cells = manager->getMap()->GetCellsInfo();
	cellManager = std::make_unique<ASCellManager>(cells,cellCountW, cellCountH, start, goal);
	routes = cellManager->GetRoute();
	routePhase = 0;
	if (routes.size() != 0)
	{
		routePoint = CalcPosition(routePhase);
		routePhase++;
	}
}

void ASAI::start()
{
	 const auto& ccc = getComponent<CharacterCommonComponents>();
	 manager = ccc->getManager();
     if (!manager) {
		 ccc->callbackSetManager([&] { manager = getComponent<CharacterCommonComponents>()->getManager(); });
     }
	//transform().setPosition(Vector3(-90, 0, -90));
}

void ASAI::originalUpdate()
{
	if (routes.size() == 0/*||currentP.x!=goalP.x|| currentP.y != goalP.y*/
		|| goal.x!= VectorToPosition(GetNearEnemy()).x
		|| goal.y != VectorToPosition(GetNearEnemy()).y)
	{
		Initialize();
	}
	else
	{
		Vector3 v3 = routePoint - transform().getPosition();
		float distance = v3.length();
		v3.normalize();
		transform().translate(v3 / 3);
		if (distance < 1 && routePhase < routes.size())
		{
			routePoint = CalcPosition(routePhase);
			routePhase++;
		}
	}
}

Vector3 ASAI::GetNearEnemy()
{
	Vector3 v;
	
	float distance = 99999;
	if (gameObject().tag() == "Enemy")
	{
		for (const CharacterPtr &character:manager->getCharacters())
		{
			const float d = Vector3().distance(transform().getPosition(), character.get()->transform().getPosition());
			if (distance > d)
			{
				distance = d;
				v = character->transform().getPosition();
			}
		}
	}
	else
	{
		for (const CharacterPtr& character : manager->getEnemys())
		{
			const float d = Vector3().distance(transform().getPosition(), character.get()->transform().getPosition());
			if (distance > d)
			{
				distance = d;
				v = character->transform().getPosition();
			}
		}
	}
	return v;
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

Position ASAI::VectorToPosition(const Vector3& v)
{
	Position p;
	float cellSize = mapWidth / cellCountW;
	p.x = (v.x + (mapWidth / 2)) / (cellSize);
	cellSize = mapHeight / cellCountH;
	p.y = (v.z + (mapHeight / 2)) / (cellSize);
	return p;
}
