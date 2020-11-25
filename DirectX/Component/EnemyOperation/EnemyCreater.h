#pragma once

#include "../Component.h"
#include <list>

class CharacterCommonComponents;

//エネミー生成クラス
class EnemyCreater : public Component {
    using CharacterPtr = std::shared_ptr<CharacterCommonComponents>;
    using CharacterPtrList = std::list<CharacterPtr>;

public:
    EnemyCreater(GameObject& gameObject);
    ~EnemyCreater();
    void createEnemys(CharacterPtrList& enemys, int stageNo);

private:
    EnemyCreater(const EnemyCreater&) = delete;
    EnemyCreater& operator=(const EnemyCreater&) = delete;
};
