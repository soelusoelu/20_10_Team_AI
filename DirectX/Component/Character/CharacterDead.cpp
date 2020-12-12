#include "CharacterDead.h"
#include "../Other/HitPointComponent.h"
#include "../../GameObject/GameObject.h"

CharacterDead::CharacterDead(GameObject& gameObject)
    : Component(gameObject)
    , mHp(nullptr)
{
}

CharacterDead::~CharacterDead() = default;

void CharacterDead::start() {
    mHp = getComponent<HitPointComponent>();
    mHp->callbackUpdateHP([&] { onUpdateHp(); });
}

void CharacterDead::onUpdateHp() {
    if (mHp->getHP() <= 0) {
        gameObject().setActive(false);
    }
}
