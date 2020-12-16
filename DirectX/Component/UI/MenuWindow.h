#pragma once

#include "../Component.h"
#include <memory>

class MenuToStageSelect;
class SpriteComponent;

class MenuWindow : public Component {
public:
    MenuWindow(GameObject& gameObject);
    ~MenuWindow();
    virtual void start() override;
    //メニューボタンが押された際の処理
    void onClickMenuButton();

private:
    MenuWindow(const MenuWindow&) = delete;
    MenuWindow& operator=(const MenuWindow&) = delete;

private:
    std::shared_ptr<MenuToStageSelect> mToStageSelect;
};
