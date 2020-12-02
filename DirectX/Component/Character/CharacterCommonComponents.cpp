#include "CharacterCommonComponents.h"
#include "CharacterAction.h"
#include "PhaseChangeSaver.h"
#include "../Collider/AABBCollider.h"
#include "../Mesh/MeshOutLine.h"
#include "../Other/HitPointComponent.h"

CharacterCommonComponents::CharacterCommonComponents(GameObject& gameObject)
    : Component(gameObject)
    , mManager(nullptr)
    , mMesh(nullptr)
    , mCollider(nullptr)
    , mHP(nullptr)
    , mCharaAction(nullptr)
    , mPhaseChangeSaver(nullptr)
    , mCost(0)
{
}

CharacterCommonComponents::~CharacterCommonComponents() = default;

void CharacterCommonComponents::start() {
    mMesh = getComponent<MeshOutLine>();
    //初期状態はアウトラインなし
    mMesh->setActiveOutLine(false);
    mCollider = getComponent<AABBCollider>();
    mHP = getComponent<HitPointComponent>();
    mCharaAction = getComponent<CharacterAction>();
    mPhaseChangeSaver = getComponent<PhaseChangeSaver>();
}

MeshOutLine& CharacterCommonComponents::getMeshOutLine() const {
    return *mMesh;
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
}

const ICharacterManager& CharacterCommonComponents::getManager() const {
    return *mManager;
}

void CharacterCommonComponents::setCost(int cost) {
    mCost = cost;
}

int CharacterCommonComponents::getCost() const {
    return mCost;
}
