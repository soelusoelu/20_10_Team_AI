#pragma once

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

private:
    CharacterCommonComponents(const CharacterCommonComponents&) = delete;
    CharacterCommonComponents& operator=(const CharacterCommonComponents&) = delete;

private:
    std::shared_ptr<MeshOutLine> mMesh;
    std::shared_ptr<AABBCollider> mCollider;
    std::shared_ptr<CharacterAction> mCharaAction;
};
