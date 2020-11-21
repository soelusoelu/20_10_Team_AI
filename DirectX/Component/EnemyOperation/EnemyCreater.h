#pragma once

#include "../Component.h"
#include <list>

//エネミー生成クラス
class EnemyCreater : public Component {
    using GameObjectPtr = std::shared_ptr<GameObject>;
    using GameObjectPtrList = std::list<GameObjectPtr>;

public:
    EnemyCreater(GameObject& gameObject);
    ~EnemyCreater();
    void createEnemys(GameObjectPtrList& enemys, int stageNo);

private:
    EnemyCreater(const EnemyCreater&) = delete;
    EnemyCreater& operator=(const EnemyCreater&) = delete;
};
