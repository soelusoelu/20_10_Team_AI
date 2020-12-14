#include "Title.h"
#include "../Sprite/SpriteComponent.h"
#include "../Text/Text.h"
#include "../../Input/Input.h"
#include "../../Sprite/SpriteUtility.h"
#include "../../System/Game.h"
#include "../../Transform/Transform2D.h"
#include "../../Utility/LevelLoader.h"

Title::Title(GameObject& gameObject) :
    Scene(gameObject)
    , mSelectingNo(0)
    , mSpriteFileName()
    , mStartPosition(Vector2::zero)
    , mNextSpace(Vector2::zero)
    , mScale(Vector2::zero)
    , mPivot(Pivot::NONE)
    , mSelectingAlpha(0.f) {
}

Title::~Title() = default;

void Title::start() {
    const auto& texts = getComponents<Text>();
    for (size_t i = 0; i < texts.size(); i++) {
        //スプライト
        const auto& s = addComponent<SpriteComponent>("SpriteComponent");
        s->setTextureFromFileName(mSpriteFileName);
        const auto& currentSize = s->getTextureSize() * mScale;
        auto& t = s->transform();
        const auto& pos = mStartPosition + Vector2::down * i * currentSize.y + i * mNextSpace;
        t.setPosition(pos);
        t.setScale(mScale);
        t.setPivot(mPivot);

        mSprites.emplace_back(s);

        //テキスト
        const auto& text = texts[i];
        text->setColor(ColorPalette::black);
        text->setPosition(pos + Vector2::down * currentSize.y / 2.f);
        text->setPivot(Pivot::CENTER);
    }
}

void Title::update() {
    const auto& mouse = Input::mouse();

    //スプライトを選択する
    selectSprite(mouse.getMousePosition());

    //マウスクリックしていないならここで終了
    if (!mouse.getMouseButtonDown(MouseCode::LeftButton)) {
        return;
    }
    //無効な番号なら終了
    if (mSelectingNo == INVALID_NO) {
        return;
    }

    if (mSelectingNo == 0) {
        //ステージ選択シーンへ遷移
        next("StageSelect");
    } else {
        //ゲーム終了
        Game::quit();
    }
}

void Title::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getString(inObj, "spriteFileName", &mSpriteFileName);
    JsonHelper::getVector2(inObj, "startPosition", &mStartPosition);
    JsonHelper::getVector2(inObj, "nextSpace", &mNextSpace);
    JsonHelper::getVector2(inObj, "scale", &mScale);
    std::string pivot;
    if (JsonHelper::getString(inObj, "pivot", &pivot)) {
        PivotFunc::stringToPivot(pivot, &mPivot);
    }
    JsonHelper::getFloat(inObj, "selectingAlpha", &mSelectingAlpha);
}

void Title::selectSprite(const Vector2& mousePosition) {
    for (size_t i = 0; i < mSprites.size(); ++i) {
        if (SpriteUtility::contains(*mSprites[i], mousePosition)) {
            //前フレームと違うスプライトを選択していたら
            if (i != mSelectingNo) {
                changeNewAlpha(i);
                changeOldAlpha(mSelectingNo);
                mSelectingNo = i;
            }
            return;
        }
    }

    changeOldAlpha(mSelectingNo);
    mSelectingNo = INVALID_NO;
}

void Title::changeNewAlpha(int newNo) {
    mSprites[newNo]->setAlpha(mSelectingAlpha);
}

void Title::changeOldAlpha(int oldNo) {
    if (oldNo != INVALID_NO) {
        mSprites[oldNo]->setAlpha(1.f);
    }
}
