#pragma once

#include "CharacterCreateInfo.h"
#include "../Component.h"
#include "../../Math/Math.h"
#include "../../Transform/Pivot.h"
#include <memory>
#include <string>
#include <vector>

class CharacterCost;

//キャラクターを生成するクラス
class CharacterCreater : public Component {
public:
    CharacterCreater(GameObject& gameObject);
    ~CharacterCreater();
    virtual void start() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;

    //ゲームオブジェクトを生成する
    void create(std::shared_ptr<GameObject>& out, int& cost);
    //外部からデータを受け取る
    void receiveExternalData(const rapidjson::Value& inObj, int maxCost);
    //このクラスを操作中か
    bool isOperating() const;

private:
    CharacterCreater(const CharacterCreater&) = delete;
    CharacterCreater& operator=(const CharacterCreater&) = delete;

    //外部から値を受け取ってから初期化
    void initialize();
    //マウスの左ボタンを押している間の処理
    void clickingLeftMouseButton(std::shared_ptr<GameObject>& out, int& cost, const Vector2& mousePos);
    //マウスでスプライトを選択する
    bool selectSprite(const Vector2& mousePos);
    //対応するキャラクターを作成する
    std::shared_ptr<GameObject> createCharacter(int id);
    //コストオーバーしてるスプライトの操作
    void spriteCostOver();

private:
    std::shared_ptr<CharacterCost> mCost;
    //キャラクター配列
    std::vector<CharacterCreateInfo> mCharactersInfo;
    //スプライトをクリックしている状態か
    bool mClickingSprite;
    //スプライトのID
    int mClickedSpriteID;
    //最大コスト
    int mMaxCost;

    //↓ファイルから受け取る値
    //スプライトを並べる際の開始位置
    Vector2 mSpriteStartPos;
    //スプライト共通のスケール値
    Vector2 mSpriteScale;
    //スプライトを並べる際の間隔
    float mSpriteSpace;
    //スプライトのピボット位置
    Pivot mSpritePivot;
};
