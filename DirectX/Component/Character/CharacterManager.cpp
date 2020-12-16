#include "CharacterManager.h"
#include "../CharacterOperation/CharacterOperation.h"
#include "../EnemyOperation/EnemyOperation.h"
#include "../../Device/Subject.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectFactory.h"

CharacterManager::CharacterManager(GameObject& gameObject)
    : Component(gameObject)
    , mCharaOperator(nullptr)
    , mEnemyOperator(nullptr)
    , mMap(nullptr)
    , mCallbackDeadCharacter(std::make_unique<Subject>())
    , mCallbackDeadEnemy(std::make_unique<Subject>())
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

void CharacterManager::onDeadCharacter() const {
    mCallbackDeadCharacter->notify();
}

void CharacterManager::onDeadEnemy() const {
    mCallbackDeadEnemy->notify();
}

const IMap* CharacterManager::getMap() const {
    return mMap;
}

void CharacterManager::updateForOperatePhase() {
    mCharaOperator->updateForOperatePhase();
}

void CharacterManager::callbackDeadCharacter(const std::function<void()>& callback) {
    mCallbackDeadCharacter->addObserver(callback);
}

void CharacterManager::callbackDeadEnemy(const std::function<void()>& callback) {
    mCallbackDeadEnemy->addObserver(callback);
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
    const IMap* map,
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
