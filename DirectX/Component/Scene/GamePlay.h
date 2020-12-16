#pragma once

#include "Scene.h"
#include "../GameState/GameState.h"
#include <memory>

class Scene;
class CharacterManager;
class GameStart;
class GameReset;
class GameJudge;
class GameClear;
class Menu;
class Map;

class GamePlay : public Scene {
public:
    GamePlay(GameObject& gameObject);
    ~GamePlay();
    virtual void start() override;
    virtual void update() override;

    virtual void getValueFromPreviousScene(const ValuePassMap& values) override;

private:
    //ステージ番号からステージをロードする
    void loadStage();

private:
    std::shared_ptr<CharacterManager> mCharacterManager;
    std::shared_ptr<GameStart> mGameStart;
    std::shared_ptr<GameReset> mGameReset;
    std::shared_ptr<GameJudge> mGameJudge;
    std::shared_ptr<GameClear> mGameClear;
    std::shared_ptr<Menu> mMenu;
    std::shared_ptr<Map> mMap;
    GameState mState;
    int mStageNo;
};
