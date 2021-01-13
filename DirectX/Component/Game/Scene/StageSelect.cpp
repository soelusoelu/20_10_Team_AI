#include "StageSelect.h"
#include "../../Engine/Sprite/SpriteComponent.h"
#include "../../Engine/Text/TextNumber.h"
#include "../../../Input/Input.h"
#include "../../../Sprite/SpriteUtility.h"
#include "../../../Transform/Transform2D.h"
#include "../../../Utility/LevelLoader.h"
#include <string>

StageSelect::StageSelect(GameObject& gameObject)
    : Scene(gameObject)
    , mSelectingNo(0)
    , mSpriteFileName()
    , mStartPosition(Vector2::zero)
    , mNextSpace(Vector2::zero)
    , mScale(Vector2::zero)
    , mPivot(Pivot::NONE)
    , mSelectingAlpha(0.f)
{
}

StageSelect::~StageSelect() = default;

void StageSelect::awake() {
    for (size_t i = 0; i < STAGE_NUM; i++) {
        const auto& s = addComponent<SpriteComponent>("SpriteComponent");
        s->setTextureFromFileName(mSpriteFileName);
        const auto& currentSize = s->getTextureSize() * mScale;
        auto& t = s->transform();
        const auto& pos = mStartPosition + Vector2::down * i * currentSize.y + i * mNextSpace;
        t.setPosition(pos);
        t.setScale(mScale);
        t.setPivot(mPivot);

        mSprites.emplace_back(s);

        const auto& text = addComponent<TextNumber>("TextNumber");
        text->setNumber(i + 1);
        text->setPosition(pos + Vector2::down * currentSize.y / 2.f);
        text->setPivot(Pivot::CENTER);
    }
}

void StageSelect::update() {
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

    //選んだステージ番号を渡す
    addValuePassToNextScene("StageNo", mSelectingNo + 1);
    next("GamePlay");
}

void StageSelect::loadProperties(const rapidjson::Value& inObj) {
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

void StageSelect::selectSprite(const Vector2& mousePosition) {
    for (int i = 0; i < STAGE_NUM; ++i) {
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

void StageSelect::changeNewAlpha(int newNo) {
    mSprites[newNo]->setAlpha(mSelectingAlpha);
}

void StageSelect::changeOldAlpha(int oldNo) {
    if (oldNo != INVALID_NO) {
        mSprites[oldNo]->setAlpha(1.f);
    }
}
