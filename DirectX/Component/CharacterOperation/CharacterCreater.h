﻿ #pragma once

#include "CharacterCreateInfo.h"
#include "../Component.h"
#include "../../Math/Math.h"
#include <memory>
#include <string>
#include <vector>

class SpriteComponent;

//キャラクターを生成するクラス
class CharacterCreater : public Component {
    using SpriteCompPtr = std::shared_ptr<SpriteComponent>;
    using SpriteCompPtrArray = std::vector<SpriteCompPtr>;

public:
    CharacterCreater(GameObject& gameObject);
    ~CharacterCreater();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;

private:
    CharacterCreater(const CharacterCreater&) = delete;
    CharacterCreater& operator=(const CharacterCreater&) = delete;

    //マウスでスプライトを選択する
    bool selectSprite(const Vector2& mousePos);
    //対応するキャラクターを作成する
    void createCharacter(int id);

private:
    SpriteCompPtrArray mSprites;
    //キャラクター配列
    std::vector<CharacterCreateInfo> mCharactersInfo;
    //スプライトをクリックしている状態か
    bool mClickedSprite;
    //スプライトのID
    int mClickedSpriteID;
    //スプライトを並べる際の開始位置
    Vector2 mSpriteStartPos;
    //スプライト共通のスケール値
    Vector2 mSpriteScale;
    //スプライトを並べる際の間隔
    float mSpriteSpace;
};
