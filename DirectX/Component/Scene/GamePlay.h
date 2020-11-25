#pragma once

#include "../Component.h"
#include "../GameState/GameState.h"
#include <memory>

class Scene;
class CharacterManager;
class GameStart;
class Map;

class GamePlay : public Component {
public:
    GamePlay(GameObject& gameObject);
    ~GamePlay();
    virtual void start() override;
    virtual void update() override;

private:
    //前のシーンからステージ番号をもらう
    void getStageNo();
    //ステージ番号からステージをロードする
    void loadStage();

private:
    std::shared_ptr<Scene> mScene;
    std::shared_ptr<CharacterManager> mCharacterManager;
    std::shared_ptr<GameStart> mGameStart;
    std::shared_ptr<Map> mMap;
    GameState mState;
    int mStageNo;
};
