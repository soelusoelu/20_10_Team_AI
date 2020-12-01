#include "PhaseChangeSaver.h"
#include "CharacterAction.h"
#include "../../Transform/Transform3D.h"

PhaseChangeSaver::PhaseChangeSaver(GameObject& gameObject)
    : Component(gameObject)
    , mOperatePosition(Vector3::zero)
    , mOperateRotation(Quaternion::identity)
{
}

PhaseChangeSaver::~PhaseChangeSaver() = default;

void PhaseChangeSaver::start() {
    auto charaAction = getComponent<CharacterAction>();
    charaAction->callbackChangeActionPhase([&] { onChangeActionPhase(); });
    charaAction->callbackChangeOperatePhase([&] { onChangeOperatePhase(); });
}

void PhaseChangeSaver::onChangeActionPhase() {
    const auto& t = transform();
    mOperatePosition = t.getPosition();
    mOperateRotation = t.getRotation();
}

void PhaseChangeSaver::onChangeOperatePhase() {
    auto& t = transform();
    t.setPosition(mOperatePosition);
    t.setRotation(mOperateRotation);
}
