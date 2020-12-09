#include "CharacterCommonComponents.h"
#include "CharacterAction.h"
#include "PhaseChangeSaver.h"
#include "../Collider/AABBCollider.h"
#include "../Mesh/MeshComponent.h"
#include "../Mesh/MeshOutLine.h"
#include "../Other/HitPointComponent.h"
#include "../../Device/Subject.h"

CharacterCommonComponents::CharacterCommonComponents(GameObject& gameObject)
    : Component(gameObject)
    , mManager(nullptr)
    , mMesh(nullptr)
    , mOutLine(nullptr)
    , mCollider(nullptr)
    , mHP(nullptr)
    , mCharaAction(nullptr)
    , mPhaseChangeSaver(nullptr)
    , mCallbackSetManager(std::make_unique<Subject>())
    , mCost(0)
{
}

CharacterCommonComponents::~CharacterCommonComponents() = default;

void CharacterCommonComponents::start() {
    mMesh = getComponent<MeshComponent>();
    mOutLine = getComponent<MeshOutLine>();
    //初期状態はアウトラインなし
    mOutLine->setActiveOutLine(false);
    mCollider = getComponent<AABBCollider>();
    mHP = getComponent<HitPointComponent>();
    mCharaAction = getComponent<CharacterAction>();
    mPhaseChangeSaver = getComponent<PhaseChangeSaver>();
}

MeshComponent& CharacterCommonComponents::getMeshComponent() const {
    return *mMesh;
}

MeshOutLine& CharacterCommonComponents::getMeshOutLine() const {
    return *mOutLine;
}

AABBCollider& CharacterCommonComponents::getAABBCollider() const {
    return *mCollider;
}

HitPointComponent& CharacterCommonComponents::getHitPoint() const {
    return *mHP;
}

CharacterAction& CharacterCommonComponents::getCharacterAction() const {
    return *mCharaAction;
}

PhaseChangeSaver& CharacterCommonComponents::getPhaseChangeSaver() const {
    return *mPhaseChangeSaver;
}

void CharacterCommonComponents::setManager(const ICharacterManager* manager) {
    mManager = manager;

    mCallbackSetManager->notify();
}

const ICharacterManager* CharacterCommonComponents::getManager() const {
    return mManager;
}

void CharacterCommonComponents::setCost(int cost) {
    mCost = cost;
}

int CharacterCommonComponents::getCost() const {
    return mCost;
}

void CharacterCommonComponents::callbackSetManager(const std::function<void()>& callback) {
    mCallbackSetManager->addObserver(callback);
}
