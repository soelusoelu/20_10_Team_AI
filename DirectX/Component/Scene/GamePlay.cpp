#include "GamePlay.h"
#include "Scene.h"
#include "../CharacterOperation/CharacterCreater.h"
#include "../../DebugLayer/Debug.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectFactory.h"
#include "../../Input/Input.h"
#include "../../Utility/LevelLoader.h"
#include "../../Utility/StringUtil.h"

GamePlay::GamePlay(GameObject& gameObject)
    : Component(gameObject)
    , mScene(nullptr)
    , mCharaCreater(nullptr)
    , mStageNo(0)
{
}

GamePlay::~GamePlay() = default;

void GamePlay::start() {
    mScene = getComponent<Scene>();
    auto cc = GameObjectCreater::create("CharacterCreater");
    mCharaCreater = cc->componentManager().getComponent<CharacterCreater>();

    getStageNo();
    loadStage();
}

void GamePlay::update() {
    Debug::renderLine(Vector3::left * 100.f, Vector3::right * 100.f, ColorPalette::red);
    Debug::renderLine(Vector3::down * 100.f, Vector3::up * 100.f, ColorPalette::green);
    Debug::renderLine(Vector3::back * 100.f, Vector3::forward * 100.f, ColorPalette::blue);

#ifdef _DEBUG
    //リセット
    if (Input::keyboard().getKeyDown(KeyCode::R)) {
        mScene->next("GamePlay");
    }
#endif // _DEBUG
}

void GamePlay::getStageNo() {
    const auto& value = mScene->getValueFromPreviousScene("StageNo");
    if (!value.has_value()) {
        return;
    }
    if (value.type() != typeid(int)) {
        return;
    }
    mStageNo = std::any_cast<int>(value);
}

void GamePlay::loadStage() {
    rapidjson::Document doc;
    if (!LevelLoader::loadJSON(doc, "Stage.json")) {
        return;
    }
    if (!doc.IsObject()) {
        return;
    }

    auto selectStage = "Stage" + StringUtil::intToString(mStageNo);
    const auto& data = doc[selectStage.c_str()];
    if (!data.IsObject()) {
        return;
    }

    std::string ground;
    if (JsonHelper::getString(data, "Ground", &ground)) {
        GameObjectCreater::create(ground);
    }

    mCharaCreater->loadCharacter(data);
}
