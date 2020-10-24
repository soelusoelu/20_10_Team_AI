#pragma once

#include "../Component.h"
#include "../../Math/Math.h"
#include <memory>

class SpriteComponent;

//キャラクターを生成するクラス
class CharacterCreater : public Component {
public:
    CharacterCreater(GameObject& gameObject);
    ~CharacterCreater();
    virtual void start() override;
    virtual void update() override;

private:
    CharacterCreater(const CharacterCreater&) = delete;
    CharacterCreater& operator=(const CharacterCreater&) = delete;

    //マウスでスプライトを選択する
    bool selectSprite(const Vector2& mousePos);
    //対応するキャラクターを作成する
    void createCharacter(int id);

private:
    std::shared_ptr<SpriteComponent> mSprite;
    //スプライトをクリックしている状態か
    bool mClickedSprite;
};
