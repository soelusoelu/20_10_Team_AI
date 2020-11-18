#include "GameStart.h"
#include "../Sprite/SpriteComponent.h"
#include "../../Collision/Collision.h"
#include "../../Input/Input.h"
#include "../../Sprite/SpriteUtility.h"
#include "../../System/Window.h"
#include "../../Transform/Transform2D.h"

GameStart::GameStart(GameObject& gameObject)
    : Component(gameObject)
{
}

GameStart::~GameStart() = default;

void GameStart::start() {
    mSprite = getComponent<SpriteComponent>();
}

void GameStart::originalUpdate(GameState& state) {
    state = (clickSprite()) ? GameState::ACTION_PHASE : GameState::OPERATE_PHASE;
}

bool GameStart::clickSprite() {
    const auto& mouse = Input::mouse();
    if (mouse.getMouseButtonDown(MouseCode::LeftButton)) {
        return SpriteUtility::contains(*mSprite, mouse.getMousePosition());
    }

    return false;
}
