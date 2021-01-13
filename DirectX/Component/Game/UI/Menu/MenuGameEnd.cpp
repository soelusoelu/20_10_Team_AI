#include "MenuGameEnd.h"
#include "../../../Engine/Sprite/SpriteButtonComponent.h"
#include "../../../../GameObject/GameObject.h"
#include "../../../../System/Game.h"

MenuGameEnd::MenuGameEnd(GameObject& gameObject)
    : Component(gameObject)
    , mButton(nullptr)
{
}

MenuGameEnd::~MenuGameEnd() = default;

void MenuGameEnd::start() {
    mButton = getComponent<SpriteButtonComponent>();
    mButton->callbackClick([&] { Game::quit(); });

    //最初は使用しない
    gameObject().setActive(false);
}

void MenuGameEnd::onOpenMenuWindow() {
    gameObject().setActive(true);
    mButton->initialize();
}

void MenuGameEnd::onCloseMenuWindow() {
    gameObject().setActive(false);
}
