#pragma once

#include "ICharacterCreateInfo.h"
#include "../Component.h"
#include "../../Math/Math.h"
#include "../../Transform/Pivot.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>

class SpriteComponent;
class CharacterCost;
class Text;
class Subject;

//キャラクター生成のスプライト操作クラス
class CharacterCreateSpriteOperation : public Component {
    using TextPtr = std::shared_ptr<Text>;
    using TextPtrArray = std::vector<TextPtr>;

    struct SpriteInfo {
        std::shared_ptr<SpriteComponent> sprite;
        bool isActive; //残りのコストをオーバーしてるか
    };

public:
    CharacterCreateSpriteOperation(GameObject& gameObject);
    ~CharacterCreateSpriteOperation();
    virtual void start() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;

    //初期化
    void initialize();
    //コンポーネント外のアップデート
    void originalUpdate();
    //スプライトを追加する
    void addSprite(const std::string& fileName);
    //キャラクター生成情報管理者を設定する
    void setCharacterCreater(const ICharacterCreateInfo* creater);
    //このクラスを操作中か
    bool isOperating() const;
    //生成するキャラのID
    int getCreateCharacterID() const;
    //キャラクターを生成する際のコールバック
    void callbackCreateCharacter(const std::function<void()>& callback);

private:
    CharacterCreateSpriteOperation(const CharacterCreateSpriteOperation&) = delete;
    CharacterCreateSpriteOperation& operator=(const CharacterCreateSpriteOperation&) = delete;

    //マウスの左ボタンを押している間の処理
    void clickingLeftMouseButton(const Vector2& mousePos);
    //マウスでスプライトを選択する
    bool selectSprite(const Vector2& mousePos);
    //コストが更新されたら
    void onUpdateCost();

private:
    const ICharacterCreateInfo* mCreater;
    std::shared_ptr<CharacterCost> mCost;
    //キャラクターを生成する際のコールバック
    std::unique_ptr<Subject> mCallbackCreateCharacter;
    //スプライト配列
    std::vector<SpriteInfo> mSprites;
    //キャラコストテキスト表示配列
    TextPtrArray mTexts;
    //スプライトをクリックしている状態か
    bool mClickingSprite;
    //スプライトのID
    int mClickedSpriteID;

    //↓ファイルから受け取る値
    //スプライトを並べる際の開始位置
    Vector2 mSpriteStartPos;
    //スプライト共通のスケール値
    Vector2 mSpriteScale;
    //スプライトを並べる際の間隔
    float mSpriteSpace;
    //スプライトのピボット位置
    Pivot mSpritePivot;
    //非アクティブ時の透明値
    float mNonActiveAlpha;
    //コスト表示サイズ
    float mCostTextScale;
};
