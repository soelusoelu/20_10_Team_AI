#include "MenuToStageSelect.h"
#include "../Sprite/SpriteButtonComponent.h"
#include "../Scene/Scene.h"
#include "../Sprite/SpriteComponent.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectManager.h"

MenuToStageSelect::MenuToStageSelect(GameObject& gameObject)
    : Component(gameObject)
    , mCurrentScene()
    , mButton(nullptr)
{
}

MenuToStageSelect::~MenuToStageSelect() = default;

void MenuToStageSelect::start() {
    mCurrentScene = gameObject().getGameObjectManager().find("Scene")->componentManager().getComponent<Scene>();

    mButton = getComponent<SpriteButtonComponent>();
    mButton->callbackClick([&] { mCurrentScene.lock()->next("StageSelect"); });

    //最初は使用しない
    gameObject().setActive(false);
}

void MenuToStageSelect::onClickMenuButton() {
    if (gameObject().getActive()) {
        //メニューが開かれている状態で押されたら、メニューを閉じる
        gameObject().setActive(false);
    } else {
        //メニューが閉じている状態で押されたら、メニューを開く
        gameObject().setActive(true);
        mButton->initialize();
    }
}
