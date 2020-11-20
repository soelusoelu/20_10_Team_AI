#pragma once

#include "../Component.h"
#include "../GameState/GameState.h"
#include <list>
#include <memory>

class GameObject;
class CharacterCreater;
class CharacterDeleter;
class CharacterSelector;
class DragAndDropCharacter;

//キャラ操作統括クラス
class CharacterOperation : public Component {
    using GameObjectPtr = std::shared_ptr<GameObject>;
    using GameObjectPtrList = std::list<GameObjectPtr>;

public:
    CharacterOperation(GameObject& gameObject);
    ~CharacterOperation();
    virtual void awake() override;
    virtual void start() override;

    //独自アップデート
    void originalUpdate(GameState state);
    //CharacterCreaterにキャラクター情報を渡す
    void loadCharacter(const rapidjson::Value& inObj);
    //アクションモードに変わった際の処理
    void onChangeActionPhase();

private:
    CharacterOperation(const CharacterOperation&) = delete;
    CharacterOperation& operator=(const CharacterOperation&) = delete;

    //操作フェーズでの処理
    void operatePhase();
    //アクションフェーズでの処理
    void actionPhase();
    //マウスの左ボタンを押した瞬間の処理
    void clickLeftMouseButton();
    //マウスの左ボタンを押している間の処理
    void clickingLeftMouseButton();

private:
    GameObjectPtrList mCreatedCharacters;
    std::shared_ptr<GameObject> mSelectObject;
    std::shared_ptr<CharacterCreater> mCreater;
    std::shared_ptr<CharacterDeleter> mDeleter;
    std::shared_ptr<CharacterSelector> mSelector;
    std::shared_ptr<DragAndDropCharacter> mDragAndDrop;
};
