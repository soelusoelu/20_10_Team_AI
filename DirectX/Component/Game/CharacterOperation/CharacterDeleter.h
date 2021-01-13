#pragma once

#include "../../Component.h"
#include <list>
#include <memory>

class CharacterCommonComponents;
class CharacterCost;

//キャラを削除するクラス
class CharacterDeleter : public Component {
    using CharacterPtr = std::shared_ptr<CharacterCommonComponents>;
    using CharacterPtrList = std::list<CharacterPtr>;

public:
    CharacterDeleter(GameObject& gameObject);
    ~CharacterDeleter();
    virtual void start() override;
    //キャラクターを削除する
    void deleteCharacter(CharacterPtr& target, CharacterPtrList& characters);

private:
    CharacterDeleter(const CharacterDeleter&) = delete;
    CharacterDeleter& operator=(const CharacterDeleter&) = delete;

private:
    std::shared_ptr<CharacterCost> mCost;
};
