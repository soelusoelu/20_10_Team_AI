#include "CharacterOperation.h"
#include "CharacterCreater.h"
#include "CharacterDeleter.h"
#include "CharacterSelector.h"
#include "DragAndDropCharacter.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectFactory.h"

CharacterOperation::CharacterOperation(GameObject& gameObject)
    : Component(gameObject)
    , mSelectObject(nullptr)
    , mCreater(nullptr)
    , mDeleter(nullptr)
    , mSelector(nullptr)
    , mDragAndDrop(nullptr)
{
}

CharacterOperation::~CharacterOperation() = default;

void CharacterOperation::awake() {
    auto cc = GameObjectCreater::create("CharacterCreater");
    mCreater = cc->componentManager().getComponent<CharacterCreater>();
}

void CharacterOperation::start() {
    mDeleter = getComponent<CharacterDeleter>();
    mSelector = getComponent<CharacterSelector>();
    mDragAndDrop = getComponent<DragAndDropCharacter>();
}

void CharacterOperation::update() {
    auto newChara = mCreater->originalUpdate();
    if (newChara) {
        mCreatedCharacters.emplace_back(newChara);
        mSelectObject = newChara;
    } else {
        mSelector->selectGameObject(mSelectObject, mCreatedCharacters);
    }

    if (mSelectObject) {
        mDragAndDrop->dragMove(*mSelectObject);
    }
}

void CharacterOperation::loadCharacter(const rapidjson::Value& inObj) {
    mCreater->loadCharacter(inObj);
}
