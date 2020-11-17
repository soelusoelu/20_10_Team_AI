#include "Title.h"
#include "Scene.h"
#include "../../DebugLayer/Debug.h"
#include "../../GameObject/GameObjectFactory.h"
#include "../../Input/Input.h"

Title::Title(GameObject& gameObject) :
    Component(gameObject),
    mScene(nullptr)
{
}

Title::~Title() = default;

void Title::start() {
    mScene = getComponent<Scene>();
}

void Title::update() {
    if (Input::mouse().getMouseButtonDown(MouseCode::LeftButton)) {
        mScene->next("StageSelect");
    }
}
