#pragma once

#include "ICharacterManager.h"
#include "../Map/IMap.h"
#include "../../Component.h"
#include <functional>
#include <memory>

class CharacterOperation;
class EnemyOperation;
class Subject;

//全キャラクター管理クラス
class CharacterManager : public Component, public ICharacterManager {
public:
    CharacterManager(GameObject& gameObject);
    ~CharacterManager();
    virtual void awake() override;

    virtual const CharacterPtrList& getCharacters() const override;
    virtual const CharacterPtrList& getEnemys() const override;
    virtual void onDeadCharacter() const override;
    virtual void onDeadEnemy() const override;
    virtual const IMap* getMap() const override;

    //独自アップデート
    void updateForOperatePhase();
    //キャラクター死亡時のコールバック
    void callbackDeadCharacter(const std::function<void()>& callback);
    //エネミー死亡時のコールバック
    void callbackDeadEnemy(const std::function<void()>& callback);
    //アクションフェーズに変わった際の処理
    void onChangeActionPhase();
    //操作フェーズに変わった際の処理
    void onChangeOperatePhase();
    //外部から情報を受け取る
    void receiveExternalData(
        const IMap* map,
        const rapidjson::Value& inObj,
        int maxCost,
        int stageNo
    );

    //キャラクター操作クラスを取得する
    const CharacterOperation& getCharacterOperation() const;

private:
    CharacterManager(const CharacterManager&) = delete;
    CharacterManager& operator=(const CharacterManager&) = delete;

private:
    std::shared_ptr<CharacterOperation> mCharaOperator;
    std::shared_ptr<EnemyOperation> mEnemyOperator;
    const IMap* mMap;
    std::unique_ptr<Subject> mCallbackDeadCharacter;
    std::unique_ptr<Subject> mCallbackDeadEnemy;
};
