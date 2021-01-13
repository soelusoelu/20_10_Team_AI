#pragma once

#include "IMenuButton.h"
#include "../../../Component.h"
#include <memory>

class Scene;
class SpriteButtonComponent;

class MenuToStageSelect : public Component, public IMenuButton {
public:
    MenuToStageSelect(GameObject& gameObject);
    ~MenuToStageSelect();
    virtual void start() override;

    virtual void onOpenMenuWindow() override;
    virtual void onCloseMenuWindow() override;

private:
    MenuToStageSelect(const MenuToStageSelect&) = delete;
    MenuToStageSelect& operator=(const MenuToStageSelect&) = delete;

private:
    std::shared_ptr<Scene> mCurrentScene;
    std::shared_ptr<SpriteButtonComponent> mButton;
};
