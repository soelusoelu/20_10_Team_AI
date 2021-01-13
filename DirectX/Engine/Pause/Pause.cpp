#include "Pause.h"
#include "../../Device/Button.h"
#include "../../Input/Input.h"
#include "../../System/Window.h"
#include "../../Utility/LevelLoader.h"

Pause::Pause() :
    mButton(nullptr),
    mFileName(""),
    mOffset(Vector2::zero),
    mIsPausing(false) {
}

Pause::~Pause() = default;

bool Pause::isPausing() const {
    return mIsPausing;
}

void Pause::loadProperties(const rapidjson::Value & inObj) {
    const auto& obj = inObj["pause"];
    if (obj.IsObject()) {
        JsonHelper::getString(obj, "fileName", &mFileName);
        JsonHelper::getVector2(obj, "offset", &mOffset);
    }
}

void Pause::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const {
    rapidjson::Value props(rapidjson::kObjectType);
    JsonHelper::setString(alloc, &props, "fileName", mFileName);
    JsonHelper::setVector2(alloc, &props, "offset", mOffset);

    inObj.AddMember("pause", props, alloc);
}

void Pause::initialize() {
    auto pos = mOffset;
    pos.x += Window::width();
    mButton = std::make_unique<SpriteButton>(nullptr, mFileName, pos);
}

void Pause::update() {
    if (!Input::mouse().getMouseButtonDown(MouseCode::LeftButton)) {
        return;
    }

    const auto& mousePos = Input::mouse().getMousePosition();
    if (!mButton->containsPoint(mousePos)) {
        return;
    }

    //ボタンがクリックされた
    mIsPausing = !mIsPausing;
}

void Pause::drawButton(const Matrix4& proj) const {
    mButton->draw(proj);
}
