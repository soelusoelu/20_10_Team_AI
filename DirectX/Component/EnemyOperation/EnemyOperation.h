#pragma once

#include "../Component.h"
#include "../Character/ICharacterManager.h"
#include <list>
#include <memory>

class EnemyCreater;
class CharacterCommonComponents;

//エネミー操作統括クラス
class EnemyOperation : public Component {
    using CharacterPtr = std::shared_ptr<CharacterCommonComponents>;
    using CharacterPtrList = std::list<CharacterPtr>;

public:
    EnemyOperation(GameObject& gameObject);
    ~EnemyOperation();
    virtual void start() override;

    //ステージ番号を設定する
    void setStageNo(int stageNo);
    //アクションモードに変わった際の処理
    void onChangeActionPhase();
    //自身を管理するマネージャーを設定する
    void setManager(const ICharacterManager* manager);

private:
    EnemyOperation(const EnemyOperation&) = delete;
    EnemyOperation& operator=(const EnemyOperation&) = delete;

private:
    CharacterPtrList mEnemys;
    const ICharacterManager* mManager;
    std::shared_ptr<EnemyCreater> mCreater;
    int mStageNo;
};
