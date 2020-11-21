#pragma once

#include "../Component.h"
#include <list>
#include <memory>

class GameObject;
class EnemyCreater;

class EnemyOperation : public Component {
    using GameObjectPtr = std::shared_ptr<GameObject>;
    using GameObjectPtrList = std::list<GameObjectPtr>;

public:
    EnemyOperation(GameObject& gameObject);
    ~EnemyOperation();
    virtual void start() override;

    //ステージ番号を設定する
    void setStageNo(int stageNo);
    //アクションモードに変わった際の処理
    void onChangeActionPhase();

private:
    EnemyOperation(const EnemyOperation&) = delete;
    EnemyOperation& operator=(const EnemyOperation&) = delete;

private:
    GameObjectPtrList mEnemys;
    std::shared_ptr<EnemyCreater> mCreater;
    int mStageNo;
};
