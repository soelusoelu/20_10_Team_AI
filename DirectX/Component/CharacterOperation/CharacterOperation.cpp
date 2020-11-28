#include "CharacterOperation.h"
#include "CharacterCreater.h"
#include "CharacterDeleter.h"
#include "CharacterSelector.h"
#include "DragAndDropCharacter.h"
#include "../Character/CharacterCommonComponents.h"
#include "../CharacterAction/CharacterAction.h"
#include "../Mesh/MeshOutLine.h"
#include "../../DebugLayer/Debug.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectFactory.h"
#include "../../Input/Input.h"

CharacterOperation::CharacterOperation(GameObject& gameObject)
    : Component(gameObject)
    , mManager(nullptr)
    , mSelectObject(nullptr)
    , mCreater(nullptr)
    , mDeleter(nullptr)
    , mSelector(nullptr)
    , mDragAndDrop(nullptr)
{
}

CharacterOperation::~CharacterOperation() = default;

void CharacterOperation::start() {
    mCreater = getComponent<CharacterCreater>();
    mDeleter = getComponent<CharacterDeleter>();
    mSelector = getComponent<CharacterSelector>();
    mDragAndDrop = getComponent<DragAndDropCharacter>();
}

void CharacterOperation::updateForOperatePhase() {
    //キャラクターを生成する
    std::shared_ptr<GameObject> out = nullptr;
    mCreater->create(out);

    //生成していたら登録する
    if (out) {
        addCharacter(*out);
        //タグを設定する
        out->setTag("Player");
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
    //マウスの左ボタンを離した瞬間なら
    if (mouse.getMouseButtonUp(MouseCode::LeftButton)) {
        releaseLeftMouseButton();
    }

    //マウスの右ボタンを押した瞬間なら
    if (mouse.getMouseButtonDown(MouseCode::RightButton)) {
        clickRightMouseButton();
    }
}

void CharacterOperation::transferExternalDataToCharacterCreater(const rapidjson::Value& inObj, int maxCost) {
    mCreater->receiveExternalData(inObj, maxCost);
}

void CharacterOperation::onChangeActionPhase() {
    //アクションフェーズでは使用しない
    mCreater->gameObject().setActive(false);

    //全キャラクターをアクションフェーズに移行する
    for (const auto& chara : mCreatedCharacters) {
        chara->getCharacterAction().enabled();
    }
}

const CharacterPtrList& CharacterOperation::getCharacters() const {
    return mCreatedCharacters;
}

void CharacterOperation::setManager(const ICharacterManager* manager) {
    mManager = manager;
    mDragAndDrop->setManager(mManager);
}

void CharacterOperation::addCharacter(const GameObject& newChara) {
    auto temp = newChara.componentManager().getComponent<CharacterCommonComponents>();

    //CharacterCommonComponentsがないならエラー出力して終了
    if (!temp) {
        Debug::logError("Not found CharacterCommonComponents.");
        return;
    }

    //選択対象にする
    changeSelectObject(temp);
    //メッシュの青みを強くする
    temp->getMeshOutLine().setColorRatio(ColorPalette::blue);
    //マネージャーを登録する
    temp->setManager(mManager);
    //登録する
    mCreatedCharacters.emplace_back(temp);
}

void CharacterOperation::changeSelectObject(const CharacterPtr& target) {
    if (mSelectObject) {
        //アウトラインを非表示にする
        mSelectObject->getMeshOutLine().setActiveOutLine(false);
    }

    if (target) {
        //アウトラインを表示する
        target->getMeshOutLine().setActiveOutLine(true);
    }

    //選択対象を変更する
    mSelectObject = target;
}

void CharacterOperation::clickLeftMouseButton() {
    //キャラクター生成クラスが操作していたら終了
    if (mCreater->isOperating()) {
        return;
    }

    //キャラクターを選択する
    CharacterPtr out = nullptr;
    if (mSelector->selectCharacter(out, mCreatedCharacters)) {
        changeSelectObject(out);
    }
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
    mDragAndDrop->dragMove(mSelectObject->gameObject());
}

void CharacterOperation::releaseLeftMouseButton() {
    //選択対象をはずす
    //changeSelectObject(nullptr);
}

void CharacterOperation::clickRightMouseButton() {
    //選択中のキャラクターを削除する
    mDeleter->deleteCharacter(mSelectObject);
}
