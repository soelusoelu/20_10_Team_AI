#include "GameJudge.h"
#include "../../Device/Subject.h"

GameJudge::GameJudge(GameObject& gameObject)
    : Component(gameObject)
    , mCharacterManager(nullptr)
    , mCallbackSomeWin(std::make_unique<Subject>())
    , mCallbackPlayerWin(std::make_unique<Subject>())
    , mCallbackEnemyWin(std::make_unique<Subject>())
    , mPlayerSideCount(0)
    , mEnemySideCount(0)
    , mIsWinPlayerSide(true)
{
}

GameJudge::~GameJudge() = default;

void GameJudge::setCharacterManager(const ICharacterManager* manager) {
    mCharacterManager = manager;
}

void GameJudge::onChangeActionPhase() {
    mPlayerSideCount = mCharacterManager->getCharacters().size();
    mEnemySideCount = mCharacterManager->getEnemys().size();
}

void GameJudge::onDeadPlayerSide() {
    --mPlayerSideCount;

    //プレイヤー側のキャラクターが0になったらエネミーの勝利
    if (mPlayerSideCount <= 0) {
        mCallbackSomeWin->notify();
        mCallbackEnemyWin->notify();
        mIsWinPlayerSide = false;
    }
}

void GameJudge::onDeadEnemySide() {
    --mEnemySideCount;

    //エネミー側のキャラクターが0になったらプレイヤーの勝利
    if (mEnemySideCount <= 0) {
        mCallbackSomeWin->notify();
        mCallbackPlayerWin->notify();
        mIsWinPlayerSide = true;
    }
}

void GameJudge::callbackSomeWin(const std::function<void()>& callback) {
    mCallbackSomeWin->addObserver(callback);
}

void GameJudge::callbackPlayerWin(const std::function<void()>& callback) {
    mCallbackPlayerWin->addObserver(callback);
}

void GameJudge::callbackEnemyWin(const std::function<void()>& callback) {
    mCallbackEnemyWin->addObserver(callback);
}

bool GameJudge::isWinPlayerSide() const {
    return mIsWinPlayerSide;
}
