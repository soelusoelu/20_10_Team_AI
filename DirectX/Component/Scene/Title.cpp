#include "Title.h"
#include "../UI/NextStageSelect.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectFactory.h"

Title::Title(GameObject& gameObject) :
    Scene(gameObject)
{
}

Title::~Title() = default;

void Title::awake() {
    GameObjectCreater::create("GameEnd");
    const auto& nss = GameObjectCreater::create("NextStageSelect")->componentManager().getComponent<NextStageSelect>();
    nss->callbackButtonClick([&] { next("StageSelect"); });
}
