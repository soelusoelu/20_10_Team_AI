#pragma once

#include "ICharacterManager.h"
#include "../Component.h"
#include "../Map/IMap.h"
#include <memory>

class CharacterOperation;
class EnemyOperation;

//全キャラクター管理クラス
class CharacterManager : public Component, public ICharacterManager {
public:
    CharacterManager(GameObject& gameObject);
    ~CharacterManager();
    virtual void awake() override;

    virtual const CharacterPtrList& getCharacters() const override;
    virtual const CharacterPtrList& getEnemys() const override;
    virtual const IMap& getMap() const override;

    //独自アップデート
    void updateForOperatePhase();
    //アクションフェーズに変わった際の処理
    void onChangeActionPhase();
    //操作フェーズに変わった際の処理
    void onChangeOperatePhase();
    //外部から情報を受け取る
    void receiveExternalData(
        const std::shared_ptr<IMap>& map,
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
    std::shared_ptr<IMap> mMap;
};
