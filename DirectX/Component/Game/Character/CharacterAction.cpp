#include "CharacterAction.h"
#include "../AI/ASAI.h"
#include "../../../Device/Subject.h"

CharacterAction::CharacterAction(GameObject& gameObject)
    : Component(gameObject)
    , mAI(nullptr)
    , mCallbackChangeActionPhase(std::make_unique<Subject>())
    , mCallbackChangeOperatePhase(std::make_unique<Subject>())
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

    //通知を送る
    mCallbackChangeActionPhase->notify();
}

void CharacterAction::disabled() {
    mIsActive = false;

    //通知を送る
    mCallbackChangeOperatePhase->notify();
}

void CharacterAction::callbackChangeActionPhase(const std::function<void()>& callback) {
    mCallbackChangeActionPhase->addObserver(callback);
}

void CharacterAction::callbackChangeOperatePhase(const std::function<void()>& callback) {
    mCallbackChangeOperatePhase->addObserver(callback);
}
