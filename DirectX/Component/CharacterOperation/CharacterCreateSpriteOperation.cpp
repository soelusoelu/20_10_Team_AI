#include "CharacterCreateSpriteOperation.h"
#include "CharacterCost.h"
#include "../Sprite/SpriteComponent.h"
#include "../Text/Text.h"
#include "../../Device/Subject.h"
#include "../../Input/Input.h"
#include "../../Sprite/SpriteUtility.h"
#include "../../Transform/Transform2D.h"
#include "../../Utility/LevelLoader.h"
#include "../../Utility/StringUtil.h"
#include <cassert>
#include <string>

CharacterCreateSpriteOperation::CharacterCreateSpriteOperation(GameObject& gameObject)
    : Component(gameObject)
    , mCreater(nullptr)
    , mCost(nullptr)
    , mCallbackCreateCharacter(std::make_unique<Subject>())
    , mClickingSprite(false)
    , mClickedSpriteID(0)
    , mSpriteStartPos(Vector2::zero)
    , mSpriteScale(Vector2::one)
    , mSpriteSpace(0.f)
    , mSpritePivot(Pivot::LEFT_TOP)
    , mNonActiveAlpha(0.f)
    , mCostTextScale(1.f)
{
}

CharacterCreateSpriteOperation::~CharacterCreateSpriteOperation() = default;

void CharacterCreateSpriteOperation::start() {
    mCost = getComponent<CharacterCost>();
    mCost->callbackUpdateCost([&] { onUpdateCost(); });
}

void CharacterCreateSpriteOperation::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getVector2(inObj, "spriteStartPosition", &mSpriteStartPos);
    JsonHelper::getVector2(inObj, "spriteScale", &mSpriteScale);
    JsonHelper::getFloat(inObj, "spriteSpace", &mSpriteSpace);
    std::string pivot;
    if (JsonHelper::getString(inObj, "spritePivot", &pivot)) {
        PivotFunc::stringToPivot(pivot, &mSpritePivot);
    }
    JsonHelper::getFloat(inObj, "nonActiveAlpha", &mNonActiveAlpha);
    JsonHelper::getFloat(inObj, "costTextScale", &mCostTextScale);
}

void CharacterCreateSpriteOperation::initialize() {
    //スプライトの位置を調整する
    for (int i = 0; i < mSprites.size(); ++i) {
        auto& s = mSprites[i].sprite;
        auto& st = s->transform();
        auto texSize = s->getTextureSize() * mSpriteScale;
        st.setScale(mSpriteScale);
        st.setPivot(mSpritePivot);

        //スプライトの位置を計算し配置していく
        st.setPosition(mSpriteStartPos + Vector2(texSize.x * i + mSpriteSpace * i, 0.f));

        //キャラコスト表示用にテキストコンポーネントを追加する
        auto text = addComponent<Text>("Text");
        //各種設定をする
        text->setText(StringUtil::intToString(mCreater->getCharacterCreateInfo(i).cost));
        text->setPosition(st.getPosition() + Vector2::up * texSize.y);
        text->setScale(Vector2::one * mCostTextScale);
        mTexts.emplace_back(text);
    }
}

void CharacterCreateSpriteOperation::originalUpdate() {
    //マウスインターフェイスを取得
    const auto& mouse = Input::mouse();

    //マウスの左ボタンを押した瞬間だったら
    if (mouse.getMouseButtonDown(MouseCode::LeftButton)) {
        mClickingSprite = selectSprite(mouse.getMousePosition());
    }
    //マウスの左ボタンを押し続けていたら
    if (mouse.getMouseButton(MouseCode::LeftButton)) {
        clickingLeftMouseButton(mouse.getMousePosition());
    }
    //マウスの左ボタンを離した瞬間だったら
    if (mouse.getMouseButtonUp(MouseCode::LeftButton)) {
        mClickingSprite = false;
    }
}

void CharacterCreateSpriteOperation::addSprite(const std::string& fileName) {
    SpriteInfo info{};
    info.sprite = addComponent<SpriteComponent>("SpriteComponent");
    info.sprite->setTextureFromFileName(fileName);
    info.isActive = true;
    mSprites.emplace_back(info);
}

void CharacterCreateSpriteOperation::setCharacterCreater(const ICharacterCreateInfo* creater) {
    mCreater = creater;
}

bool CharacterCreateSpriteOperation::isOperating() const {
    return mClickingSprite;
}

int CharacterCreateSpriteOperation::getCreateCharacterID() const {
    return mClickedSpriteID;
}

void CharacterCreateSpriteOperation::callbackCreateCharacter(const std::function<void()>& callback) {
    mCallbackCreateCharacter->addObserver(callback);
}

void CharacterCreateSpriteOperation::clickingLeftMouseButton(const Vector2& mousePos) {
    //スプライトをクリックしていないなら終了
    if (!mClickingSprite) {
        return;
    }

    //マウス位置がクリックしたスプライトの内にあるなら終了
    if (SpriteUtility::contains(*mSprites[mClickedSpriteID].sprite, mousePos)) {
        return;
    }

    //キャラクターを生成したと通知を送る
    mCallbackCreateCharacter->notify();

    //多重生成を阻止するため
    mClickingSprite = false;
}

bool CharacterCreateSpriteOperation::selectSprite(const Vector2& mousePos) {
    //すべてのスプライトで検証する
    for (int i = 0; i < mSprites.size(); ++i) {
        auto& sprite = mSprites[i];
        //非アクティブならスキップ
        if (!sprite.isActive) {
            continue;
        }

        //スプライトの中にマウスの座標が含まれているか
        if (SpriteUtility::contains(*sprite.sprite, mousePos)) {
            mClickedSpriteID = i;
            return true;
        }
    }

    //すべてのスプライトの矩形範囲外
    return false;
}

void CharacterCreateSpriteOperation::onUpdateCost() {
    assert(mSprites.size() == mTexts.size());
    assert(mSprites.size() == mCreater->getCharacterCount());

    for (size_t i = 0; i < mSprites.size(); ++i) {
        //キャラのコストが現在のコストより多ければ使用不可にする
        auto& sprite = mSprites[i];
        if (mCreater->getCharacterCreateInfo(i).cost > mCost->getCost()) {
            sprite.sprite->setAlpha(mNonActiveAlpha);
            sprite.isActive = false;
            mTexts[i]->setAlpha(mNonActiveAlpha);
        } else {
            sprite.sprite->setAlpha(1.f);
            sprite.isActive = true;
            mTexts[i]->setAlpha(1.f);
        }
    }
}
