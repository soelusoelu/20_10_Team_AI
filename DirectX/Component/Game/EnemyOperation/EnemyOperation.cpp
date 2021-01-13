#include "EnemyOperation.h"
#include "EnemyCreater.h"
#include "../Character/CharacterAction.h"
#include "../Character/CharacterCommonComponents.h"
#include "../../ComponentManager.h"
#include "../../Engine/Mesh/MeshComponent.h"
#include "../../Engine/Mesh/MeshMaterial.h"
#include "../../../System/Texture/TextureFromMemory.h"

EnemyOperation::EnemyOperation(GameObject& gameObject)
    : Component(gameObject)
    , mManager(nullptr)
    , mCreater(nullptr)
    , mEnemyMaterial(nullptr)
{
}

EnemyOperation::~EnemyOperation() = default;

void EnemyOperation::start() {
    mCreater = getComponent<EnemyCreater>();
    mEnemyMaterial = std::make_shared<Material>();
    mEnemyMaterial->diffuse = ColorPalette::red;
    mEnemyMaterial->texture = std::make_shared<TextureFromMemory>(1, 1);
}

void EnemyOperation::receiveStageNo(int stageNo) {
    //エネミーを生成する
    mCreater->createEnemys(mEnemys, stageNo);

    for (const auto& enemy : mEnemys) {
        //メッシュの赤みを強くする
        const auto& mat = enemy->getComponent<MeshMaterial>();
        auto mesh = enemy->getMeshComponent().getMesh();
        for (unsigned i = 0; i < mesh->getMeshCount(); ++i) {
            mat->setMaterial(mEnemyMaterial, i);
        }

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
