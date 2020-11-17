#include "StageSelect.h"
#include "Scene.h"
#include "../../Input/Input.h"

StageSelect::StageSelect(GameObject& gameObject)
    : Component(gameObject)
{
}

StageSelect::~StageSelect() = default;

void StageSelect::start() {
    mScene = getComponent<Scene>();
}

void StageSelect::update() {
    if (Input::mouse().getMouseButtonDown(MouseCode::LeftButton)) {
        //選んだステージ番号を渡す
        mScene->addValuePassToNextScene("StageNo", 1);
        mScene->next("GamePlay");
    }
}
