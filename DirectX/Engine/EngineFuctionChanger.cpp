#include "EngineFuctionChanger.h"
#include "../Device/Button.h"
#include "../Input/Input.h"
#include "../Utility/LevelLoader.h"

EngineFuctionChanger::EngineFuctionChanger()
    : mCallbackChangeEngineMode(nullptr)
    , mStartRenderPosition()
    , mSpriteSpace(0.f)
{
}

EngineFuctionChanger::~EngineFuctionChanger() = default;

void EngineFuctionChanger::loadProperties(const rapidjson::Value& inObj) {
    const auto& efcObj = inObj["engineFuctionChanger"];
    if (efcObj.IsObject()) {
        JsonHelper::getVector2(efcObj, "startRenderPosition", &mStartRenderPosition);
        JsonHelper::getStringArray(efcObj, "spritesFilePath", &mSpritesFilePath);
        JsonHelper::getFloat(efcObj, "spriteSpace", &mSpriteSpace);
    }
}

void EngineFuctionChanger::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const {
    rapidjson::Value props(rapidjson::kObjectType);
    JsonHelper::setVector2(alloc, &props, "startRenderPosition", mStartRenderPosition);
    JsonHelper::setStringArray(alloc, &props, "spritesFilePath", mSpritesFilePath);
    JsonHelper::setFloat(alloc, &props, "spriteSpace", mSpriteSpace);

    inObj.AddMember("engineFuctionChanger", props, alloc);
}

void EngineFuctionChanger::initialize(ICallbackChangeEngineMode* callback) {
    mCallbackChangeEngineMode = callback;

    for (size_t i = 0; i < mSpritesFilePath.size(); ++i) {
        mSpritesButton.emplace_back(std::make_unique<SpriteButton>(nullptr, mSpritesFilePath[i], mStartRenderPosition + Vector2::right * (SPRITE_WIDTH + mSpriteSpace) * i));
    }
    mSpritesButton[0]->setClickFunc([&] { mCallbackChangeEngineMode->onChangeGameMode(); });
    mSpritesButton[1]->setClickFunc([&] { mCallbackChangeEngineMode->onChangeMapEditorMode(); });
    mSpritesButton[2]->setClickFunc([&] { mCallbackChangeEngineMode->onChangeModelViewerMode(); });
}

void EngineFuctionChanger::update() {
    for (const auto& button : mSpritesButton) {
        button->clickButton(Input::mouse().getMousePosition());
    }
}

void EngineFuctionChanger::draw(const Matrix4& proj) const {
    for (const auto& button : mSpritesButton) {
        button->draw(proj);
    }
}
