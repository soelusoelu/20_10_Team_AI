#pragma once

#include "IMenuButton.h"
#include "../../Component.h"

class Scene;
class SpriteButtonComponent;

class MenuToTitle : public Component, public IMenuButton {
public:
    MenuToTitle(GameObject& gameObject);
    ~MenuToTitle();
    virtual void start() override;

    virtual void onOpenMenuWindow() override;
    virtual void onCloseMenuWindow() override;

private:
    MenuToTitle(const MenuToTitle&) = delete;
    MenuToTitle& operator=(const MenuToTitle&) = delete;

private:
    std::shared_ptr<Scene> mCurrentScene;
    std::shared_ptr<SpriteButtonComponent> mButton;
};
