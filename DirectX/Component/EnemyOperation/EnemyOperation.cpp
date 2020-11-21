#include "EnemyOperation.h"
#include "EnemyCreater.h"
#include "../ComponentManager.h"
#include "../CharacterAction/CharacterAction.h"
#include "../../GameObject/GameObject.h"

EnemyOperation::EnemyOperation(GameObject& gameObject)
    : Component(gameObject)
    , mCreater(nullptr)
    , mStageNo(0)
{
}

EnemyOperation::~EnemyOperation() = default;

void EnemyOperation::start() {
    mCreater = getComponent<EnemyCreater>();
    mCreater->createEnemys(mEnemys, mStageNo);
}

void EnemyOperation::setStageNo(int stageNo) {
    mStageNo = stageNo;
}

void EnemyOperation::onChangeActionPhase() {
    //全エネミーをアクションフェーズに移行する
    for (const auto& enemy : mEnemys) {
        auto action = enemy->componentManager().getComponent<CharacterAction>();
        action->enabled();
    }
}
