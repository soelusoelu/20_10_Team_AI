#pragma once

#include "../../Component.h"
#include <memory>

class MenuWindow;

class Menu : public Component {
public:
    Menu(GameObject& gameObject);
    ~Menu();
    virtual void start() override;

private:
    Menu(const Menu&) = delete;
    Menu& operator=(const Menu&) = delete;

    //メニューボタンが押された際の処理
    void onClickMenuButton();

private:
    std::shared_ptr<MenuWindow> mMenuWindow;
};
