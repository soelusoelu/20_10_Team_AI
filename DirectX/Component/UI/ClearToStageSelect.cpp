#include "ClearToStageSelect.h"
#include "../Scene/Scene.h"
#include "../Sprite/SpriteButtonComponent.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectManager.h"

ClearToStageSelect::ClearToStageSelect(GameObject& gameObject)
    : Component(gameObject)
{
}

ClearToStageSelect::~ClearToStageSelect() = default;

void ClearToStageSelect::start() {
    mCurrentScene = gameObject().getGameObjectManager().find("Scene")->componentManager().getComponent<Scene>();

    mButton = getComponent<SpriteButtonComponent>();
    mButton->callbackClick([&] { mCurrentScene->next("StageSelect"); });

    //最初は使用しない
    gameObject().setActive(false);
}

void ClearToStageSelect::onStageClear() {
    gameObject().setActive(true);
    mButton->initialize();
}
