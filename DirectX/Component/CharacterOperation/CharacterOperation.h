#pragma once

#include "../Component.h"
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
    virtual void update() override;
    void loadCharacter(const rapidjson::Value& inObj);

private:
    CharacterOperation(const CharacterOperation&) = delete;
    CharacterOperation& operator=(const CharacterOperation&) = delete;

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
