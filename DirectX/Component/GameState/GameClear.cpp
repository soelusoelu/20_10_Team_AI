﻿#include "GameClear.h"
#include "StageClear.h"
#include "StageFail.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectFactory.h"

GameClear::GameClear(GameObject& gameObject)
    : Component(gameObject)
    , mStageClear(nullptr)
    , mStageFail(nullptr)
{
}

GameClear::~GameClear() = default;

void GameClear::start() {
    mStageClear = GameObjectCreater::create("StageClear")->componentManager().getComponent<StageClear>();
    mStageFail = GameObjectCreater::create("StageFail")->componentManager().getComponent<StageFail>();

    //最初は使用しない
    gameObject().setActive(false);
}

void GameClear::initialize() {
    gameObject().setActive(false);
    mStageClear->initialize();
    mStageFail->initialize();
}

void GameClear::updatePlayerWin() {

}

void GameClear::updateEnemyWin() {
    mStageFail->originalUpdate();
}

void GameClear::onWinPlayerSide() {
    gameObject().setActive(true);
    mStageClear->onWinPlayerSide();
}

void GameClear::onWinEnemySide() {
    gameObject().setActive(true);
    mStageFail->onWinEnemySide();
}

StageClear& GameClear::getStageClear() const {
    return *mStageClear;
}

StageFail& GameClear::getStageFail() const {
    return *mStageFail;
}
