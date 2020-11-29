#include "CharacterCommonComponents.h"
#include "../CharacterAction/CharacterAction.h"
#include "../Collider/AABBCollider.h"
#include "../Mesh/MeshOutLine.h"

CharacterCommonComponents::CharacterCommonComponents(GameObject& gameObject)
    : Component(gameObject)
    , mManager(nullptr)
    , mMesh(nullptr)
    , mCollider(nullptr)
    , mCharaAction(nullptr)
    , mCost(0)
{
}

CharacterCommonComponents::~CharacterCommonComponents() = default;

void CharacterCommonComponents::start() {
    mMesh = getComponent<MeshOutLine>();
    //初期状態はアウトラインなし
    mMesh->setActiveOutLine(false);
    mCollider = getComponent<AABBCollider>();
    mCharaAction = getComponent<CharacterAction>();
}

MeshOutLine& CharacterCommonComponents::getMeshOutLine() const {
    return *mMesh;
}

AABBCollider& CharacterCommonComponents::getAABBCollider() const {
    return *mCollider;
}

CharacterAction& CharacterCommonComponents::getCharacterAction() const {
    return *mCharaAction;
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
