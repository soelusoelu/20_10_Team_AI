#include "CharacterDeleter.h"

CharacterDeleter::CharacterDeleter(GameObject& gameObject)
    : Component(gameObject)
{
}

CharacterDeleter::~CharacterDeleter() = default;
