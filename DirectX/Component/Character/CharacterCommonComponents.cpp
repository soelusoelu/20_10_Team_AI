#include "CharacterCommonComponents.h"
#include "../CharacterAction/CharacterAction.h"
#include "../Collider/AABBCollider.h"
#include "../Mesh/MeshOutLine.h"

CharacterCommonComponents::CharacterCommonComponents(GameObject& gameObject)
    : Component(gameObject)
    , mMesh(nullptr)
    , mCollider(nullptr)
    , mCharaAction(nullptr)
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
