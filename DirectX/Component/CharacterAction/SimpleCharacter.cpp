#include "SimpleCharacter.h"
#include "CharacterAction.h"
#include "../Mesh/SkinMeshComponent.h"

SimpleCharacter::SimpleCharacter(GameObject& gameObject)
    : Component(gameObject)
    , mSkinMesh(nullptr)
{
}

SimpleCharacter::~SimpleCharacter() = default;

void SimpleCharacter::start() {
    mSkinMesh = getComponent<SkinMeshComponent>();
    mSkinMesh->setMotionName("Idol", 0);
    mSkinMesh->setMotionName("Walk", 1);

    //コールバック登録
    getComponent<CharacterAction>()->callbackChangeActionPhase(this);
}

void SimpleCharacter::onChangeActionPhase() {
    mSkinMesh->changeMotion("Walk");
}
