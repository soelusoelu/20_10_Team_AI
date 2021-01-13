#include "MenuToStageSelect.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../Engine/Sprite/SpriteButtonComponent.h"
#include "../../../../GameObject/GameObject.h"
#include "../../../../GameObject/GameObjectManager.h"

MenuToStageSelect::MenuToStageSelect(GameObject& gameObject)
    : Component(gameObject)
    , mCurrentScene(nullptr)
    , mButton(nullptr)
{
}

MenuToStageSelect::~MenuToStageSelect() = default;

void MenuToStageSelect::start() {
    mCurrentScene = gameObject().getGameObjectManager().find("Scene")->componentManager().getComponent<Scene>();

    mButton = getComponent<SpriteButtonComponent>();
    mButton->callbackClick([&] { mCurrentScene->next("StageSelect"); });

    //最初は使用しない
    gameObject().setActive(false);
}

void MenuToStageSelect::onOpenMenuWindow() {
    gameObject().setActive(true);
    mButton->initialize();
}

void MenuToStageSelect::onCloseMenuWindow() {
    gameObject().setActive(false);
}
