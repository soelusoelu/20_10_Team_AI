#include "StageClear.h"
#include "../Sprite/SpriteComponent.h"
#include "../../GameObject/GameObject.h"

StageClear::StageClear(GameObject& gameObject)
    : Component(gameObject)
    , mBackground(nullptr)
    , mStageClear(nullptr)
{
}

StageClear::~StageClear() = default;

void StageClear::start() {
    const auto& sprites = getComponents<SpriteComponent>();
    mBackground = sprites[BACKGROUND];
    mStageClear = sprites[STAGE_CLEAR];

    gameObject().setActive(false);
}

void StageClear::initialize() {
    gameObject().setActive(false);
}

void StageClear::onWinPlayerSide() {
    gameObject().setActive(true);
}
