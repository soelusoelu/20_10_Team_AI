#pragma once

//各メニューボタンインターフェース
class IMenuButton {
public:
    virtual ~IMenuButton() = default;
    //メニューウィンドウが開かれたときに呼ばれる処理
    virtual void onOpenMenuWindow() = 0;
    //メニューウィンドウが閉じたときに呼ばれる処理
    virtual void onCloseMenuWindow() = 0;
};
