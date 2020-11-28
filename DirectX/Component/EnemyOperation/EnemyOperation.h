﻿#pragma once

#include "../Component.h"
#include "../Character/ICharacterManager.h"
#include <list>
#include <memory>

class EnemyCreater;
class CharacterCommonComponents;

//エネミー操作統括クラス
class EnemyOperation : public Component {
public:
    EnemyOperation(GameObject& gameObject);
    ~EnemyOperation();
    virtual void start() override;

    //ステージ番号を受け取る
    void receiveStageNo(int stageNo);
    //アクションモードに変わった際の処理
    void onChangeActionPhase();
    //全エネミーを取得する
    const CharacterPtrList& getEnemys() const;
    //自身を管理するマネージャーを設定する
    void setManager(const ICharacterManager* manager);

private:
    EnemyOperation(const EnemyOperation&) = delete;
    EnemyOperation& operator=(const EnemyOperation&) = delete;

private:
    CharacterPtrList mEnemys;
    const ICharacterManager* mManager;
    std::shared_ptr<EnemyCreater> mCreater;
};
