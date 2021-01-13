#pragma once

#include "IMenuButton.h"
#include "../../../Component.h"
#include <memory>

class MenuWindow;
class SpriteButtonComponent;

class MenuClose : public Component, public IMenuButton {
public:
    MenuClose(GameObject& gameObject);
    ~MenuClose();
    virtual void start() override;

    virtual void onOpenMenuWindow() override;
    virtual void onCloseMenuWindow() override;

    //メニューウィンドウをセットする
    void setMenuWindow(const std::shared_ptr<MenuWindow>& menuWindow);

private:
    MenuClose(const MenuClose&) = delete;
    MenuClose& operator=(const MenuClose&) = delete;

private:
    std::shared_ptr<MenuWindow> mMenuWindow;
    std::shared_ptr<SpriteButtonComponent> mButton;
};
