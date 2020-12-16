#include "CharacterOperation.h"
#include "CharacterCost.h"
#include "CharacterCreater.h"
#include "CharacterCreateSpriteOperation.h"
#include "CharacterDeleter.h"
#include "CharacterSelector.h"
#include "DragAndDropCharacter.h"
#include "../Character/CharacterAction.h"
#include "../Character/CharacterCommonComponents.h"
#include "../Mesh/MeshComponent.h"
#include "../Mesh/MeshMaterial.h"
#include "../Mesh/MeshOutLine.h"
#include "../../DebugLayer/Debug.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectFactory.h"
#include "../../Input/Input.h"
#include "../../System/Texture/TextureFromMemory.h"

CharacterOperation::CharacterOperation(GameObject& gameObject)
    : Component(gameObject)
    , mManager(nullptr)
    , mSelectObject(nullptr)
    , mCreater(nullptr)
    , mDeleter(nullptr)
    , mSelector(nullptr)
    , mDragAndDrop(nullptr)
    , mCharacterMaterial(nullptr)
{
}

CharacterOperation::~CharacterOperation() = default;

void CharacterOperation::start() {
    mCreater = getComponent<CharacterCreater>();
    mDeleter = getComponent<CharacterDeleter>();
    mSelector = getComponent<CharacterSelector>();
    mDragAndDrop = getComponent<DragAndDropCharacter>();

    mCharacterMaterial = std::make_shared<Material>();
    mCharacterMaterial->diffuse = ColorPalette::blue;
    mCharacterMaterial->texture = std::make_shared<TextureFromMemory>(1, 1);

    getComponent<CharacterCreateSpriteOperation>()->callbackCreateCharacter([&] { onCreateCharacter(); });
}

void CharacterOperation::updateForOperatePhase() {
    mCreater->originalUpdate();

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
    //最大コストを設定する
    getComponent<CharacterCost>()->setCost(maxCost);
    //データを受け渡す
    mCreater->receiveExternalData(inObj);
}

void CharacterOperation::onChangeActionPhase() {
    //アクションフェーズでは使用しない
    gameObject().setActive(false);
    //アウトラインを非表示にする
    if (mSelectObject) {
        mSelectObject->getMeshOutLine().setActiveOutLine(false);
    }

    //全キャラクターをアクションフェーズに移行する
    for (const auto& chara : mCreatedCharacters) {
        chara->getCharacterAction().enabled();
    }
}

void CharacterOperation::onChangeOperatePhase() {
    //操作フェーズで使用する
    gameObject().setActive(true);

    //全キャラクターをアクション待機状態にする
    for (const auto& chara : mCreatedCharacters) {
        chara->getCharacterAction().disabled();
    }
}

const CharacterPtrList& CharacterOperation::getCharacters() const {
    return mCreatedCharacters;
}

void CharacterOperation::setManager(const ICharacterManager* manager) {
    mManager = manager;
    mDragAndDrop->setManager(mManager);
}

void CharacterOperation::clickLeftMouseButton() {
    //キャラクター生成クラスが操作していたら終了
    if (mCreater->isOperating()) {
        return;
    }

    //キャラクターを選択する
    CharacterPtr out = nullptr;
    mSelector->selectCharacter(out, mCreatedCharacters);
    changeSelectObject(out);
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

void CharacterOperation::releaseLeftMouseButton() {
    //選択対象をはずす
    //changeSelectObject(nullptr);
}

void CharacterOperation::clickRightMouseButton() {
    //キャラクターを選択していないなら終了
    if (!mSelectObject) {
        return;
    }

    //選択中のキャラクターを削除する
    mDeleter->deleteCharacter(mSelectObject, mCreatedCharacters);
    //選択対象をなしにする
    changeSelectObject(nullptr);
}

void CharacterOperation::onCreateCharacter() {
    //キャラクターを生成する
    std::shared_ptr<GameObject> out = nullptr;
    int cost = 0;
    mCreater->create(out, cost);

    //生成していたら登録する
    if (out) {
        addCharacter(*out, cost);
        //タグを設定する
        out->setTag("Player");
    }
}

void CharacterOperation::addCharacter(const GameObject& newChara, int cost) {
    auto temp = newChara.componentManager().getComponent<CharacterCommonComponents>();

    //CharacterCommonComponentsがないならエラー出力して終了
    if (!temp) {
        Debug::logError("Not found CharacterCommonComponents.");
        return;
    }

    //選択対象にする
    changeSelectObject(temp);
    //コストを設定する
    temp->setCost(cost);
    //メッシュの青みを強くする
    const auto& mat = temp->getComponent<MeshMaterial>();
    auto mesh = temp->getMeshComponent().getMesh();
    for (unsigned i = 0; i < mesh->getMeshCount(); ++i) {
        mat->setMaterial(mCharacterMaterial, i);
    }
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
