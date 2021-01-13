#include "MenuClose.h"
#include "MenuWindow.h"
#include "../../../Engine/Sprite/SpriteButtonComponent.h"
#include "../../../../GameObject/GameObject.h"

MenuClose::MenuClose(GameObject& gameObject)
    : Component(gameObject)
    , mMenuWindow(nullptr)
    , mButton(nullptr)
{
}

MenuClose::~MenuClose() = default;

void MenuClose::start() {
    mButton = getComponent<SpriteButtonComponent>();

    //最初は使用しない
    gameObject().setActive(false);
}

void MenuClose::onOpenMenuWindow() {
    gameObject().setActive(true);
    mButton->initialize();
}

void MenuClose::onCloseMenuWindow() {
    gameObject().setActive(false);
}

void MenuClose::setMenuWindow(const std::shared_ptr<MenuWindow>& menuWindow) {
    mMenuWindow = menuWindow;
    mButton->callbackClick([&] { mMenuWindow->close(); });
}
