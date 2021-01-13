#include "MenuWindow.h"
#include "../../../../GameObject/GameObject.h"

MenuWindow::MenuWindow(GameObject& gameObject)
    : Component(gameObject)
{
}

MenuWindow::~MenuWindow() = default;

void MenuWindow::start() {
    //最初は使用しない
    gameObject().setActive(false);
}

void MenuWindow::addMenuButton(IMenuButton* button) {
    mButtons.emplace_back(button);
}

void MenuWindow::open() {
    gameObject().setActive(true);

    //全ボタンにウィンドウが開かれたことを通知する
    for (const auto& b : mButtons) {
        b->onOpenMenuWindow();
    }
}

void MenuWindow::close() {
    gameObject().setActive(false);

    //全ボタンにウィンドウが閉じたことを通知する
    for (const auto& b : mButtons) {
        b->onCloseMenuWindow();
    }
}

bool MenuWindow::isOpen() const {
    //ウィンドウが開かれているかは、自身のゲームオブジェクトのアクティブ状態によって判定する
    return gameObject().getActive();
}
