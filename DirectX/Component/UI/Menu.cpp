#include "Menu.h"
#include "MenuWindow.h"
#include "../Sprite/SpriteButtonComponent.h"
#include "../Sprite/SpriteComponent.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectFactory.h"
#include "../../Sprite/SpriteUtility.h"

Menu::Menu(GameObject& gameObject)
    : Component(gameObject)
    , mMenuWindow(nullptr)
{
}

Menu::~Menu() = default;

void Menu::start() {
    mMenuWindow = GameObjectCreater::create("MenuWindow")->componentManager().getComponent<MenuWindow>();

    const auto& button = getComponent<SpriteButtonComponent>();
    button->callbackClick([&] { mMenuWindow->onClickMenuButton(); });
}
