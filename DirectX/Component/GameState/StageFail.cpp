#include "StageFail.h"
#include "StageFailArrow.h"
#include "../../GameObject/GameObject.h"

StageFail::StageFail(GameObject& gameObject)
    : Component(gameObject)
    , mArrow(nullptr)
{
}

StageFail::~StageFail() = default;

void StageFail::start() {
    mArrow = getComponent<StageFailArrow>();

    gameObject().setActive(false);
}

void StageFail::originalUpdate() {
    mArrow->originalUpdate();
}

void StageFail::initialize() {
    gameObject().setActive(false);
}

void StageFail::onWinEnemySide() {
    gameObject().setActive(true);
}

StageFailArrow& StageFail::getFailArrow() const {
    return *mArrow;
}
