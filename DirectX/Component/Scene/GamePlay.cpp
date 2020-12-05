#include "GamePlay.h"
#include "Scene.h"
#include "../Character/CharacterManager.h"
#include "../CharacterOperation/CharacterOperation.h"
#include "../CharacterOperation/DragAndDropCharacter.h"
#include "../GameState/GameReset.h"
#include "../GameState/GameStart.h"
#include "../Map/Map.h"
#include "../../DebugLayer/Debug.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectFactory.h"
#include "../../Input/Input.h"
#include "../../Utility/LevelLoader.h"
#include "../../Utility/StringUtil.h"

GamePlay::GamePlay(GameObject& gameObject)
    : Scene(gameObject)
    , mCharacterManager(nullptr)
    , mGameStart(nullptr)
    , mGameReset(nullptr)
    , mMap(nullptr)
    , mState(GameState::OPERATE_PHASE)
    , mStageNo(0)
{
}

GamePlay::~GamePlay() = default;

void GamePlay::start() {
    mCharacterManager = getComponent<CharacterManager>();
    mMap = getComponent<Map>();

    //GameObjectCreater::create("SphereMap");

    auto gs = GameObjectCreater::create("GameStart");
    mGameStart = gs->componentManager().getComponent<GameStart>();

    auto gr = GameObjectCreater::create("GameReset");
    mGameReset = gr->componentManager().getComponent<GameReset>();

    mGameStart->callbackGameStart([&] { mState = GameState::ACTION_PHASE; });
    mGameStart->callbackGameStart([&] { mCharacterManager->onChangeActionPhase(); });
    mGameStart->callbackGameStart([&] { mGameReset->onChangeActionPhase(); });

    mGameReset->callbackGameReset([&] { mState = GameState::OPERATE_PHASE; });
    mGameReset->callbackGameReset([&] { mCharacterManager->onChangeOperatePhase(); });
    mGameReset->callbackGameReset([&] { mGameStart->onChangeOperatePhase(); });
}

void GamePlay::update() {
    if (mState == GameState::OPERATE_PHASE) {
        mCharacterManager->updateForOperatePhase();
        mGameStart->originalUpdate();
    } else if (mState == GameState::ACTION_PHASE) {
        mGameReset->originalUpdate();
    }

    Debug::renderLine(Vector3::left * 100.f, Vector3::right * 100.f, ColorPalette::red);
    Debug::renderLine(Vector3::down * 100.f, Vector3::up * 100.f, ColorPalette::green);
    Debug::renderLine(Vector3::back * 100.f, Vector3::forward * 100.f, ColorPalette::blue);

#ifdef _DEBUG
    //リセット
    if (Input::keyboard().getKeyDown(KeyCode::R)) {
        next("GamePlay");
    }
#endif // _DEBUG
}

void GamePlay::getValueFromPreviousScene(const ValuePassMap& values) {
    const auto& value = values.at("StageNo");
    if (!value.has_value()) {
        return;
    }
    if (value.type() != typeid(int)) {
        return;
    }
    mStageNo = std::any_cast<int>(value);

    loadStage();
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
