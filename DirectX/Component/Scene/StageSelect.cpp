#include "StageSelect.h"
#include "../Sprite/SpriteComponent.h"
#include "../Text/TextNumber.h"
#include "../../Input/Input.h"
#include "../../Sprite/SpriteUtility.h"
#include "../../Transform/Transform2D.h"
#include "../../Utility/LevelLoader.h"
#include <string>

StageSelect::StageSelect(GameObject& gameObject)
    : Scene(gameObject)
    , mStartPosition(Vector2::zero)
    , mNextSpace(Vector2::zero)
    , mScale(Vector2::zero)
    , mPivot(Pivot::NONE)
{
}

StageSelect::~StageSelect() = default;

void StageSelect::update() {
    const auto& mouse = Input::mouse();
    if (!mouse.getMouseButtonDown(MouseCode::LeftButton)) {
        return;
    }

    const int SPRITE_COUNT = static_cast<int>(mSprites.size());
    for (int i = 0; i < SPRITE_COUNT; ++i) {
        if (SpriteUtility::contains(*mSprites[i], mouse.getMousePosition())) {
            //選んだステージ番号を渡す
            addValuePassToNextScene("StageNo", i + 1);
            next("GamePlay");
        }
    }
}

void StageSelect::loadProperties(const rapidjson::Value& inObj) {
    std::string fileName;
    JsonHelper::getString(inObj, "spriteFileName", &fileName);
    JsonHelper::getVector2(inObj, "startPosition", &mStartPosition);
    JsonHelper::getVector2(inObj, "nextSpace", &mNextSpace);
    JsonHelper::getVector2(inObj, "scale", &mScale);
    std::string pivot;
    if (JsonHelper::getString(inObj, "pivot", &pivot)) {
        PivotFunc::stringToPivot(pivot, &mPivot);
    }

    for (size_t i = 0; i < 3; i++) {
        const auto& s = addComponent<SpriteComponent>("SpriteComponent");
        s->setTextureFromFileName(fileName);
        const auto& size = s->getTextureSize();
        const auto& currentSize = size * mScale;
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
