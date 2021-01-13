#include "Menu.h"
#include "MenuWindow.h"
#include "../../../Engine/Sprite/SpriteButtonComponent.h"
#include "../../../Engine/Sprite/SpriteComponent.h"
#include "../../../../GameObject/GameObject.h"
#include "../../../../GameObject/GameObjectFactory.h"
#include "../../../../Sprite/SpriteUtility.h"

Menu::Menu(GameObject& gameObject)
    : Component(gameObject)
    , mMenuWindow(nullptr)
    , mButton(nullptr)
{
}

Menu::~Menu() = default;

void Menu::start() {
    mMenuWindow = GameObjectCreater::create("MenuWindow")->componentManager().getComponent<MenuWindow>();

    mButton = getComponent<SpriteButtonComponent>();
    mButton->callbackClick([&] { onClickMenuButton(); });
}

void Menu::onWinPlayerSide() {
    mButton->enableButtonFunction(false);
}

void Menu::onWinEnemySide() {
    mButton->enableButtonFunction(true);
}

void Menu::onClickMenuButton() {
    if (mMenuWindow->isOpen()) {
        //メニューウィンドウが開かれている状態で押されたら、ウィンドウを閉じる
        mMenuWindow->close();
    } else {
        //メニューウィンドウが閉じている状態で押されたら、ウィンドウを開く
        mMenuWindow->open();
    }
}
