#pragma once

#include "IMenuButton.h"
#include "../../../Component.h"
#include <memory>

class SpriteButtonComponent;

class MenuGameEnd : public Component, public IMenuButton {
public:
    MenuGameEnd(GameObject& gameObject);
    ~MenuGameEnd();
    virtual void start() override;

    virtual void onOpenMenuWindow() override;
    virtual void onCloseMenuWindow() override;

private:
    MenuGameEnd(const MenuGameEnd&) = delete;
    MenuGameEnd& operator=(const MenuGameEnd&) = delete;

private:
    std::shared_ptr<SpriteButtonComponent> mButton;
};
