#pragma once

#include "../Component.h"
#include "../GameState/GameState.h"
#include <memory>

class Scene;
class CharacterOperation;
class EnemyCreater;
class GameStart;

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
    std::shared_ptr<CharacterOperation> mCharaOperator;
    std::shared_ptr<EnemyCreater> mEnemyCreater;
    std::shared_ptr<GameStart> mGameStart;
    GameState mState;
    int mStageNo;
};
