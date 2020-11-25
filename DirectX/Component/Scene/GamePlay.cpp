﻿#include "GamePlay.h"
#include "Scene.h"
#include "../Character/CharacterManager.h"
#include "../GameState/GameStart.h"
#include "../Map/Map.h"
#include "../../DebugLayer/Debug.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectFactory.h"
#include "../../Input/Input.h"
#include "../../Utility/LevelLoader.h"
#include "../../Utility/StringUtil.h"

GamePlay::GamePlay(GameObject& gameObject)
    : Component(gameObject)
    , mScene(nullptr)
    , mCharacterManager(nullptr)
    , mGameStart(nullptr)
    , mMap(nullptr)
    , mState(GameState::OPERATE_PHASE)
    , mStageNo(0)
{
}

GamePlay::~GamePlay() = default;

void GamePlay::start() {
    mScene = getComponent<Scene>();
    mCharacterManager = getComponent<CharacterManager>();
    mMap = getComponent<Map>();

    GameObjectCreater::create("SphereMap");

    auto gs = GameObjectCreater::create("GameStart");
    mGameStart = gs->componentManager().getComponent<GameStart>();

    mGameStart->callbackGameStart([this] { mState = GameState::ACTION_PHASE; });
    mGameStart->callbackGameStart([&] { mCharacterManager->onChangeActionPhase(); });

    getStageNo();
    loadStage();
}

void GamePlay::update() {
    if (mState == GameState::OPERATE_PHASE) {
        mCharacterManager->updateForOperatePhase();
        mGameStart->originalUpdate();
    } else if (mState == GameState::ACTION_PHASE) {

    }

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

    //地形データを渡す
    std::string ground;
    JsonHelper::getString(data, "ground", &ground);
    mMap->receiveMapData(ground);

    //コストを取得
    int cost;
    JsonHelper::getInt(data, "cost", &cost);

    //情報を渡す
    mCharacterManager->receiveExternalData(mMap, data, cost, mStageNo);
}
