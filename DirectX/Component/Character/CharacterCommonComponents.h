#pragma once

#include "ICharacterManager.h"
#include "../Component.h"
#include <functional>
#include <memory>

class MeshComponent;
class MeshOutLine;
class AABBCollider;
class HitPointComponent;
class CharacterAction;
class PhaseChangeSaver;
class Subject;

//キャラクター共通コンポーネント管理クラス
class CharacterCommonComponents : public Component {
public:
    CharacterCommonComponents(GameObject& gameObject);
    ~CharacterCommonComponents();
    virtual void start() override;

    MeshComponent& getMeshComponent() const;
    MeshOutLine& getMeshOutLine() const;
    AABBCollider& getAABBCollider() const;
    HitPointComponent& getHitPoint() const;
    CharacterAction& getCharacterAction() const;
    PhaseChangeSaver& getPhaseChangeSaver() const;

    //自身を管理するマネージャーを設定する
    void setManager(const ICharacterManager* manager);
    //自身を管理するマネージャーを取得する
    const ICharacterManager* getManager() const;
    //生成時コストを設定する
    void setCost(int cost);
    //生成時コストを取得する
    int getCost() const;

    //マネージャーが登録された際のコールバック
    void callbackSetManager(const std::function<void()>& callback);

private:
    CharacterCommonComponents(const CharacterCommonComponents&) = delete;
    CharacterCommonComponents& operator=(const CharacterCommonComponents&) = delete;

private:
    const ICharacterManager* mManager;
    std::shared_ptr<MeshComponent> mMesh;
    std::shared_ptr<MeshOutLine> mOutLine;
    std::shared_ptr<AABBCollider> mCollider;
    std::shared_ptr<HitPointComponent> mHP;
    std::shared_ptr<CharacterAction> mCharaAction;
    std::shared_ptr<PhaseChangeSaver> mPhaseChangeSaver;
    std::unique_ptr<Subject> mCallbackSetManager;
    int mCost;
};
