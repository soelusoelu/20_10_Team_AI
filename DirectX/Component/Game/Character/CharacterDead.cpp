#include "CharacterDead.h"
#include "CharacterCommonComponents.h"
#include "../Other/HitPointComponent.h"
#include "../../../Device/Subject.h"
#include "../../../GameObject/GameObject.h"

CharacterDead::CharacterDead(GameObject& gameObject)
    : Component(gameObject)
    , mManager(nullptr)
    , mHp(nullptr)
    , mCallbackDeadPlayer(std::make_unique<Subject>())
    , mCallbackDeadEnemy(std::make_unique<Subject>())
{
}

CharacterDead::~CharacterDead() = default;

void CharacterDead::start() {
    mHp = getComponent<HitPointComponent>();
    mHp->callbackUpdateHP([&] { onUpdateHp(); });

    //キャラクターマネージャー登録時にコールバックを設定する
    getComponent<CharacterCommonComponents>()->callbackSetManager([&] {
        mManager = getComponent<CharacterCommonComponents>()->getManager();
        callbackDeadPlayer([&] { mManager->onDeadCharacter(); });
        callbackDeadEnemy([&] { mManager->onDeadEnemy(); });
    });
}

void CharacterDead::callbackDeadPlayer(const std::function<void()>& callback) {
    mCallbackDeadPlayer->addObserver(callback);
}

void CharacterDead::callbackDeadEnemy(const std::function<void()>& callback) {
    mCallbackDeadEnemy->addObserver(callback);
}

void CharacterDead::onUpdateHp() {
    if (mHp->getHP() <= 0) {
        //死亡
        gameObject().setActive(false);

        //死亡したことを知らせる
        const auto& tag = gameObject().tag();
        if (tag == "Player") {
            mCallbackDeadPlayer->notify();
        } else if (tag == "Enemy") {
            mCallbackDeadEnemy->notify();
        }
    }
}
