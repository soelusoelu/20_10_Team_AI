#include "GameEnd.h"
#include "../Sprite/SpriteButtonComponent.h"
#include "../Sprite/SpriteComponent.h"
#include "../Text/Text.h"
#include "../../System/Game.h"
#include "../../Transform/Transform2D.h"

GameEnd::GameEnd(GameObject& gameObject)
    : Component(gameObject)
{
}

GameEnd::~GameEnd() = default;

void GameEnd::start() {
    const auto& s = getComponent<SpriteComponent>();
    const auto& text = getComponent<Text>();
    text->setPosition(s->transform().getPosition());

    //このボタンはゲーム終了
    getComponent<SpriteButtonComponent>()->callbackClick([&] { Game::quit(); });
}
