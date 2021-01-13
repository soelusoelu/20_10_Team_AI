#include "PhaseChangeSaver.h"
#include "CharacterAction.h"
#include "../Other/HitPointComponent.h"
#include "../../../GameObject/GameObject.h"
#include "../../../Transform/Transform3D.h"

PhaseChangeSaver::PhaseChangeSaver(GameObject& gameObject)
    : Component(gameObject)
    , mHP(nullptr)
    , mOperatePosition(Vector3::zero)
    , mOperateRotation(Quaternion::identity)
    , mOriginalHP(0)
{
}

PhaseChangeSaver::~PhaseChangeSaver() = default;

void PhaseChangeSaver::start() {
    mHP = getComponent<HitPointComponent>();
    auto charaAction = getComponent<CharacterAction>();
    charaAction->callbackChangeActionPhase([&] { onChangeActionPhase(); });
    charaAction->callbackChangeOperatePhase([&] { onChangeOperatePhase(); });
}

void PhaseChangeSaver::onChangeActionPhase() {
    const auto& t = transform();
    mOperatePosition = t.getPosition();
    mOperateRotation = t.getRotation();
    mOriginalHP = mHP->getMaxHP();
}

void PhaseChangeSaver::onChangeOperatePhase() {
    auto& obj = gameObject();
    if (!obj.getActive()) {
        obj.setActive(true);
    }
    auto& t = transform();
    t.setPosition(mOperatePosition);
    t.setRotation(mOperateRotation);
    mHP->setHP(mOriginalHP);
}
