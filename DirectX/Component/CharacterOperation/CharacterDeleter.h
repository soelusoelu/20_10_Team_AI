#pragma once

#include "../Component.h"
#include <memory>

class CharacterCommonComponents;

//キャラを削除するクラス
class CharacterDeleter : public Component {
    using CharacterPtr = std::shared_ptr<CharacterCommonComponents>;

public:
    CharacterDeleter(GameObject& gameObject);
    ~CharacterDeleter();
    //キャラクターを削除する
    void deleteCharacter(CharacterPtr& target);

private:
    CharacterDeleter(const CharacterDeleter&) = delete;
    CharacterDeleter& operator=(const CharacterDeleter&) = delete;
};
