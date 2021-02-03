#include "ASAI.h"
#include "../../../Transform/Transform3D.h"
#include "../../Engine/Collider/AABBCollider.h"

#include "../../../Math/Vector3.h"
#include "ASCellManager.h"
#include "../../../GameObject/GameObject.h"
#include "../Character/CharacterCommonComponents.h"
#include "../Character/ICharacterManager.h"
#include "../Other/HitPointComponent.h"
#include "../../../Math/Math.h"

class CharacterCommonComponents;

ASAI::ASAI(GameObject& obj):Component(obj)
{
}

ASAI::~ASAI()
{
}

void ASAI::Initialize()
{
	time = Time(0.5);
	Position start = VectorToPosition(transform().getPosition());
	start.x = fmaxf(0,fminf( start.x, cellCountW - 1));
	start.y = fmaxf(0, fminf(start.y,cellCountH-1));
	routePoint = GetNearEnemy();
	goal = VectorToPosition(routePoint);
	cells = manager->getMap()->GetCellsInfo();
	cellManager = std::make_unique<ASCellManager>(cells,cellCountW, cellCountH, start, goal);
	//cellManager = std::make_unique<ASCellManager>(cells, cellCountW, cellCountH, start, goal);

	routes = cellManager->GetRoute();
	routePhase = 0;
	avoidObstacle = cellManager->avoidObstacle;
	if (avoidObstacle)
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
	collider=getComponent<AABBCollider>();
}

void ASAI::onCollisionStay(Collider& other) {
    if (other.gameObject().tag() != gameObject().tag()
		&&time.isTime()) {
		other.getComponent<HitPointComponent>()->takeDamage(10);
		if (other.getComponent<HitPointComponent>()->getHP() <= 0)
		{
			Initialize();
		}
		time.reset();
    }
}

void ASAI::onCollisionExit(Collider& other) {
	if (other.gameObject().tag() != gameObject().tag())
	{
		Initialize();
	}
}

void ASAI::originalUpdate()
{
	//‚à‚µ‚à“G‚Ì‘¶Ý‚·‚éƒZƒ‹‚ª•ÏX‚³‚ê‚½‚çÄ’Tõ‚·‚é
	Position enemyPos = VectorToPosition(GetNearEnemy());
	if (routes.size() == 0/*||currentP.x!=goalP.x|| currentP.y != goalP.y*/
		|| goal.x!= enemyPos.x
		|| goal.y != enemyPos.y)
	{
		Initialize();
	}
	else
	//“G‚É‚«‚í‚ß‚Ä‹ß‚¢ê‡‚Í’¼ÚÚ‹ßA‚»‚êˆÈŠO‚Íƒ‹[ƒg‚ð‚½‚Ç‚é
	{
		Vector3 v3 = routePoint - transform().getPosition();
		float distance = v3.length();
		v3.normalize();
		if (distance > 1)
		{
			transform().translate(v3 / 2);
		}
		if (distance < 1 && routePhase < routes.size()&&avoidObstacle)
		{
			routePoint = CalcPosition(routePhase);
			routePhase++;
		}
		if (!avoidObstacle)
		{
			routePoint = GetNearEnemy();
		}
		//if (!collider->onCollisionExit().empty())
		//{
		//	Initialize();
		//}
	}
	time.update();
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
			if (distance > d&&character.get()->getComponent<HitPointComponent>()->getHP()!=0)
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
