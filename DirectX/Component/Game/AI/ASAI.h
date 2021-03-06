#pragma once
#include <vector>
#include "ASCell.h"
#include "ASCellManager.h"
#include "../../Component.h"
#include "../../Engine/Collider/AABBCollider.h"
#include "../../../Math/Vector3.h"
#include "../Character/ICharacterManager.h"
#include "../DirectX/Device/Time.h"

class CharacterCommonComponents;


class ASAI :public Component
{
public:
	ASAI(GameObject& obj);
	~ASAI();
	void Initialize();
	virtual void start()override;
	virtual void onCollisionStay(Collider& other) override;
	virtual void onCollisionExit(Collider& other) override;
	void originalUpdate();

	//最近の敵の情報を取得する
	Vector3 GetNearEnemy();

	//float JudgeCharacter(CharacterPtr chara, float distance);
private:
	std::shared_ptr<Collider> collider;
private:
	Vector3 CalcPosition(int phase);

	Position VectorToPosition(const Vector3& v);

	std::unique_ptr<ASCellManager> cellManager;
	std::unique_ptr < ASCellManager> target;
	std::vector<Position> routes;
	Time time;
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
	bool avoidObstacle;
};

