#include "EnemyOperation.h"
#include "EnemyCreater.h"
#include "../ComponentManager.h"
#include "../Character/CharacterAction.h"
#include "../Character/CharacterCommonComponents.h"
#include "../Mesh/MeshComponent.h"

EnemyOperation::EnemyOperation(GameObject& gameObject)
    : Component(gameObject)
    , mManager(nullptr)
    , mCreater(nullptr)
{
}

EnemyOperation::~EnemyOperation() = default;

void EnemyOperation::start() {
    mCreater = getComponent<EnemyCreater>();
}

void EnemyOperation::receiveStageNo(int stageNo) {
    //エネミーを生成する
    mCreater->createEnemys(mEnemys, stageNo);

    for (const auto& enemy : mEnemys) {
        //メッシュの赤みを強くする
        enemy->getMeshComponent().setColorRatio(ColorPalette::red);
        //マネージャーを設定する
        enemy->setManager(mManager);
    }
}

void EnemyOperation::onChangeActionPhase() {
    //全エネミーをアクションフェーズに移行する
    for (const auto& enemy : mEnemys) {
        enemy->getCharacterAction().enabled();
    }
}

void EnemyOperation::onChangeOperatePhase() {
    //全エネミーをアクション待機状態にする
    for (const auto& enemy : mEnemys) {
        enemy->getCharacterAction().disabled();
    }
}

const CharacterPtrList& EnemyOperation::getEnemys() const {
    return mEnemys;
}

void EnemyOperation::setManager(const ICharacterManager* manager) {
    mManager = manager;
}
