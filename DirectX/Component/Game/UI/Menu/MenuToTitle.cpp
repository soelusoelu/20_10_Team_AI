#include "MenuToTitle.h"
#include "../../../Engine/Sprite/SpriteButtonComponent.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../../GameObject/GameObject.h"
#include "../../../../GameObject/GameObjectManager.h"

MenuToTitle::MenuToTitle(GameObject& gameObject)
    : Component(gameObject)
    , mCurrentScene(nullptr)
    , mButton(nullptr) {
}

MenuToTitle::~MenuToTitle() = default;

void MenuToTitle::start() {
    mCurrentScene = gameObject().getGameObjectManager().find("Scene")->componentManager().getComponent<Scene>();

    mButton = getComponent<SpriteButtonComponent>();
    mButton->callbackClick([&] { mCurrentScene->next("Title"); });

    //最初は使用しない
    gameObject().setActive(false);
}

void MenuToTitle::onOpenMenuWindow() {
    gameObject().setActive(true);
    mButton->initialize();
}

void MenuToTitle::onCloseMenuWindow() {
    gameObject().setActive(false);
}
