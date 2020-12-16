#pragma once

#include "../Component.h"
#include <memory>

class Scene;
class SpriteButtonComponent;

class MenuToStageSelect : public Component {
public:
    MenuToStageSelect(GameObject& gameObject);
    ~MenuToStageSelect();
    virtual void start() override;
    //メニューボタンが押された際の処理
    void onClickMenuButton();

private:
    MenuToStageSelect(const MenuToStageSelect&) = delete;
    MenuToStageSelect& operator=(const MenuToStageSelect&) = delete;

private:
    std::weak_ptr<Scene> mCurrentScene;
    std::shared_ptr<SpriteButtonComponent> mButton;
};
