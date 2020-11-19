#include "CharacterOperation.h"
#include "CharacterCreater.h"
#include "CharacterDeleter.h"
#include "CharacterSelector.h"
#include "DragAndDropCharacter.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectFactory.h"
#include "../../Input/Input.h"

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
    //キャラクターを生成する
    auto newChara = mCreater->create();

    if (newChara) {
        //生成していたら登録し、選択対象にする
        mCreatedCharacters.emplace_back(newChara);
        mSelectObject = newChara;
    }

    //マウスインターフェイスを取得
    const auto& mouse = Input::mouse();

    //マウスの左ボタンを押した瞬間なら
    if (mouse.getMouseButtonDown(MouseCode::LeftButton)) {
        clickLeftMouseButton();
    }
    //マウスの左ボタンを押し続けていたら
    if (mouse.getMouseButton(MouseCode::LeftButton)) {
        clickingLeftMouseButton();
    }
}

void CharacterOperation::loadCharacter(const rapidjson::Value& inObj) {
    mCreater->loadCharacter(inObj);
}

void CharacterOperation::clickLeftMouseButton() {
    //キャラクター生成クラスが操作していたら終了
    if (mCreater->isOperating()) {
        return;
    }

    //ゲームオブジェクトを選択する
    mSelector->selectGameObject(mSelectObject, mCreatedCharacters);
}

void CharacterOperation::clickingLeftMouseButton() {
    //キャラクター生成クラスが操作していたら終了
    if (mCreater->isOperating()) {
        return;
    }
    //オブジェクトが選択されていなかったら終了
    if (!mSelectObject) {
        return;
    }

    //選択しているゲームオブジェクトを移動する
    mDragAndDrop->dragMove(*mSelectObject);
}
