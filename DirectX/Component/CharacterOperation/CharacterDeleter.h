#pragma once

#include "../Component.h"
#include <memory>

//キャラを削除するクラス
class CharacterDeleter : public Component {
public:
    CharacterDeleter(GameObject& gameObject);
    ~CharacterDeleter();

private:
    CharacterDeleter(const CharacterDeleter&) = delete;
    CharacterDeleter& operator=(const CharacterDeleter&) = delete;

private:
    std::shared_ptr<GameObject> mTarget;
};
