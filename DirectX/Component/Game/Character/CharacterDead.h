#pragma once

#include "ICharacterManager.h"
#include "../../Component.h"
#include <functional>
#include <memory>

class HitPointComponent;
class Subject;

//キャラクターの生死を扱うクラス
class CharacterDead : public Component {
public:
    CharacterDead(GameObject& gameObject);
    ~CharacterDead();
    virtual void start() override;
    //プレイヤー側のキャラクターが死亡した際のコールバック
    void callbackDeadPlayer(const std::function<void()>& callback);
    //エネミー側のキャラクターが死亡した際のコールバック
    void callbackDeadEnemy(const std::function<void()>& callback);
    //HPが更新されたら
    void onUpdateHp();

private:
    CharacterDead(const CharacterDead&) = delete;
    CharacterDead& operator=(const CharacterDead&) = delete;

private:
    const ICharacterManager* mManager;
    std::shared_ptr<HitPointComponent> mHp;
    std::unique_ptr<Subject> mCallbackDeadPlayer;
    std::unique_ptr<Subject> mCallbackDeadEnemy;
};
