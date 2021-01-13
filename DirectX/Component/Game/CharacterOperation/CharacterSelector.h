#pragma once

#include "../../Component.h"
#include <list>
#include <memory>

class Camera;
class CharacterCommonComponents;

//キャラクターを選択するクラス
class CharacterSelector : public Component {
    using CharacterPtr = std::shared_ptr<CharacterCommonComponents>;
    using CharacterPtrList = std::list<CharacterPtr>;

public:
    CharacterSelector(GameObject& gameObject);
    ~CharacterSelector();
    virtual void start() override;
    //キャラクターを選択する
    bool selectCharacter(
        CharacterPtr& out,
        const CharacterPtrList& characters
    ) const;

private:
    CharacterSelector(const CharacterSelector&) = delete;
    CharacterSelector& operator=(const CharacterSelector&) = delete;

private:
    std::shared_ptr<Camera> mCamera;
};
