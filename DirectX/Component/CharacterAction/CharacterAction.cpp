#include "CharacterAction.h"
#include "../AI/ASAI.h"

CharacterAction::CharacterAction(GameObject& gameObject)
    : Component(gameObject)
    , mAI(nullptr)
    , mIsActive(false)
{
}

CharacterAction::~CharacterAction() = default;

void CharacterAction::start() {
    mAI = getComponent<ASAI>();
}

void CharacterAction::update() {
    if (!mIsActive) {
        return;
    }
    if (!mAI) {
        return;
    }

    //AIにすべてをまかせる
    mAI->originalUpdate();
}

void CharacterAction::onEnable(bool value) {
    mIsActive = value;
}

void CharacterAction::enabled() {
    mIsActive = true;
}

void CharacterAction::disabled() {
    mIsActive = false;
}
