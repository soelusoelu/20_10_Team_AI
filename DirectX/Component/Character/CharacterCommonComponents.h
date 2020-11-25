#pragma once

#include "ICharacterManager.h"
#include "../Component.h"
#include <memory>

class MeshOutLine;
class AABBCollider;
class CharacterAction;

//キャラクター共通コンポーネント管理クラス
class CharacterCommonComponents : public Component {
public:
    CharacterCommonComponents(GameObject& gameObject);
    ~CharacterCommonComponents();
    virtual void start() override;
    MeshOutLine& getMeshOutLine() const;
    AABBCollider& getAABBCollider() const;
    CharacterAction& getCharacterAction() const;

    //自身を管理するマネージャーを設定する
    void setManager(const ICharacterManager* manager);
    //自身を管理するマネージャーを取得する
    const ICharacterManager& getManager() const;

private:
    CharacterCommonComponents(const CharacterCommonComponents&) = delete;
    CharacterCommonComponents& operator=(const CharacterCommonComponents&) = delete;

private:
    const ICharacterManager* mManager;
    std::shared_ptr<MeshOutLine> mMesh;
    std::shared_ptr<AABBCollider> mCollider;
    std::shared_ptr<CharacterAction> mCharaAction;
};
