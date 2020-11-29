#include "CharacterDeleter.h"
#include "CharacterCost.h"
#include "../Character/CharacterCommonComponents.h"
#include "../../GameObject/GameObject.h"

CharacterDeleter::CharacterDeleter(GameObject& gameObject)
    : Component(gameObject)
    , mCost(nullptr)
{
}

CharacterDeleter::~CharacterDeleter() = default;

void CharacterDeleter::start() {
    mCost = getComponent<CharacterCost>();
}

void CharacterDeleter::deleteCharacter(CharacterPtr& target, CharacterPtrList& characters) {
    //生成コスト分返却する
    mCost->returnCost(target->getCost());
    //削除
    target->gameObject().destroy();
    //リストからも削除
    characters.remove(target);
}
