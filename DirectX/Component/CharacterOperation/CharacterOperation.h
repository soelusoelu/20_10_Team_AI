#pragma once

#include "../Component.h"
#include "../Character/ICharacterManager.h"
#include "../GameState/GameState.h"
#include <list>
#include <memory>

class GameObject;
class CharacterCreater;
class CharacterDeleter;
class CharacterSelector;
class DragAndDropCharacter;
class CharacterCommonComponents;

//キャラ操作統括クラス
class CharacterOperation : public Component {
    using CharacterPtr = std::shared_ptr<CharacterCommonComponents>;
    using CharacterPtrList = std::list<CharacterPtr>;

public:
    CharacterOperation(GameObject& gameObject);
    ~CharacterOperation();
    virtual void awake() override;
    virtual void start() override;

    //独自アップデート
    void updateForOperatePhase();
    //CharacterCreaterに情報を渡す
    void transferExternalDataToCharacterCreater(const rapidjson::Value& inObj, int maxCost);
    //アクションモードに変わった際の処理
    void onChangeActionPhase();
    //自身を管理するマネージャーを設定する
    void setManager(const ICharacterManager* manager);

private:
    CharacterOperation(const CharacterOperation&) = delete;
    CharacterOperation& operator=(const CharacterOperation&) = delete;

    //新しいキャラクターを登録する
    void addCharacter(const GameObject& newChara);
    //マウスの左ボタンを押した瞬間の処理
    void clickLeftMouseButton();
    //マウスの左ボタンを押している間の処理
    void clickingLeftMouseButton();
    //マウスの左ボタンを離した瞬間の処理
    void releaseLeftMouseButton();
    //選択中メッシュのアウトラインを描画するか
    void setOutLineForSelectObject(bool value);

private:
    CharacterPtrList mCreatedCharacters;
    const ICharacterManager* mManager;
    std::shared_ptr<CharacterCommonComponents> mSelectObject;
    std::shared_ptr<CharacterCreater> mCreater;
    std::shared_ptr<CharacterDeleter> mDeleter;
    std::shared_ptr<CharacterSelector> mSelector;
    std::shared_ptr<DragAndDropCharacter> mDragAndDrop;
};
