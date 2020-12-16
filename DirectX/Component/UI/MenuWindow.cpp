#include "MenuWindow.h"
#include "MenuToStageSelect.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectFactory.h"

MenuWindow::MenuWindow(GameObject& gameObject)
    : Component(gameObject)
    , mToStageSelect(nullptr)
{
}

MenuWindow::~MenuWindow() = default;

void MenuWindow::start() {
    mToStageSelect = GameObjectCreater::create("MenuToStageSelect")->componentManager().getComponent<MenuToStageSelect>();

    //最初は使用しない
    gameObject().setActive(false);
}

void MenuWindow::onClickMenuButton() {
    if (gameObject().getActive()) {
        //メニューが開かれている状態で押されたら、メニューを閉じる
        gameObject().setActive(false);
    } else {
        //メニューが閉じている状態で押されたら、メニューを開く
        gameObject().setActive(true);
    }

    mToStageSelect->onClickMenuButton();
}
