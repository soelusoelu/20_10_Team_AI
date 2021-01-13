#pragma once

#include "IMenuButton.h"
#include "../../../Component.h"
#include <list>

class MenuWindow : public Component {
public:
    MenuWindow(GameObject& gameObject);
    ~MenuWindow();
    virtual void start() override;

    //メニューボタンを追加する
    void addMenuButton(IMenuButton* button);
    //メニューウィンドウを開く
    void open();
    //メニューウィンドウを閉じる
    void close();
    //メニューウィンドウが開かれているか
    bool isOpen() const;

private:
    MenuWindow(const MenuWindow&) = delete;
    MenuWindow& operator=(const MenuWindow&) = delete;

private:
    std::list<IMenuButton*> mButtons;
};
