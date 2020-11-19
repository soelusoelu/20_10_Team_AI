#pragma once

#include "../Component.h"
#include <list>
#include <memory>

class Camera;

//キャラクターを選択するクラス
class CharacterSelector : public Component {
    using GameObjectPtr = std::shared_ptr<GameObject>;
    using GameObjectPtrList = std::list<GameObjectPtr>;

public:
    CharacterSelector(GameObject& gameObject);
    ~CharacterSelector();
    virtual void start() override;
    void selectGameObject(GameObjectPtr& out, const GameObjectPtrList& characters) const;

private:
    CharacterSelector(const CharacterSelector&) = delete;
    CharacterSelector& operator=(const CharacterSelector&) = delete;

private:
    std::shared_ptr<Camera> mCamera;
};
