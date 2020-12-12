#pragma once

#include "../Component.h"
#include <memory>

class HitPointComponent;

//キャラクターの生死を扱うクラス
class CharacterDead : public Component {
public:
    CharacterDead(GameObject& gameObject);
    ~CharacterDead();
    virtual void start() override;
    //HPが更新されたら
    void onUpdateHp();

private:
    CharacterDead(const CharacterDead&) = delete;
    CharacterDead& operator=(const CharacterDead&) = delete;

private:
    std::shared_ptr<HitPointComponent> mHp;
};
