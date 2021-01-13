#include "SimpleCharacter.h"
#include "CharacterAction.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"

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
    auto charaAction = getComponent<CharacterAction>();
    charaAction->callbackChangeActionPhase([&] { onChangeActionPhase(); });
    charaAction->callbackChangeOperatePhase([&] { onChangeOperatePhase(); });
}

void SimpleCharacter::onChangeActionPhase() {
    mSkinMesh->changeMotion("Walk");
}

void SimpleCharacter::onChangeOperatePhase() {
    mSkinMesh->changeMotion("Idol");
}
