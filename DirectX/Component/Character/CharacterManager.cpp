#include "CharacterManager.h"
#include "../CharacterOperation/CharacterOperation.h"
#include "../EnemyOperation/EnemyOperation.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectFactory.h"

CharacterManager::CharacterManager(GameObject& gameObject)
    : Component(gameObject)
    , mCharaOperator(nullptr)
    , mEnemyOperator(nullptr)
    , mMap(nullptr)
{
}

CharacterManager::~CharacterManager() = default;

void CharacterManager::awake() {
    auto co = GameObjectCreater::create("CharacterOperation");
    mCharaOperator = co->componentManager().getComponent<CharacterOperation>();
    mCharaOperator->setManager(this);

    auto eo = GameObjectCreater::create("EnemyOperation");
    mEnemyOperator = eo->componentManager().getComponent<EnemyOperation>();
    mEnemyOperator->setManager(this);
}

const CharacterPtrList& CharacterManager::getCharacters() const {
    return mCharaOperator->getCharacters();
}

const CharacterPtrList& CharacterManager::getEnemys() const {
    return mEnemyOperator->getEnemys();
}

const IMap& CharacterManager::getMap() const {
    return *mMap;
}

void CharacterManager::updateForOperatePhase() {
    mCharaOperator->updateForOperatePhase();
}

void CharacterManager::onChangeActionPhase() {
    mCharaOperator->onChangeActionPhase();
    mEnemyOperator->onChangeActionPhase();
}

void CharacterManager::onChangeOperatePhase() {
    mCharaOperator->onChangeOperatePhase();
    mEnemyOperator->onChangeOperatePhase();
}

void CharacterManager::receiveExternalData(
    const std::shared_ptr<IMap>& map,
    const rapidjson::Value& inObj,
    int maxCost,
    int stageNo
) {
    mMap = map;
    mCharaOperator->transferExternalDataToCharacterCreater(inObj, maxCost);
    mEnemyOperator->receiveStageNo(stageNo);
}

const CharacterOperation& CharacterManager::getCharacterOperation() const {
    return *mCharaOperator;
}
