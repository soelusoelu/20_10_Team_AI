#pragma once

#include "../../Component.h"
#include <memory>

class MenuWindow;
class SpriteButtonComponent;

class Menu : public Component {
public:
    Menu(GameObject& gameObject);
    ~Menu();
    virtual void start() override;

    //ステージクリアした際の処理
    void onChangeStageClearPhase();

private:
    Menu(const Menu&) = delete;
    Menu& operator=(const Menu&) = delete;

    //メニューボタンが押された際の処理
    void onClickMenuButton();

private:
    std::shared_ptr<MenuWindow> mMenuWindow;
    std::shared_ptr<SpriteButtonComponent> mButton;
};
