#include "StageSelect.h"
#include "../../Input/Input.h"

StageSelect::StageSelect(GameObject& gameObject)
    : Scene(gameObject)
{
}

StageSelect::~StageSelect() = default;

void StageSelect::update() {
    if (Input::mouse().getMouseButtonDown(MouseCode::LeftButton)) {
        //選んだステージ番号を渡す
        addValuePassToNextScene("StageNo", 1);
        next("GamePlay");
    }
}
