#include "FixedDebugInformation.h"
#include "../../../Device/DrawString.h"
#include "../../../System/Window.h"
#include "../../../Utility/LevelLoader.h"
#include "../../../Utility/StringUtil.h"

FixedDebugInformation::FixedDebugInformation() :
    mFpsGetter(nullptr),
    mScale(Vector2::one),
    mFPSPos(Vector2::zero) {
}

FixedDebugInformation::~FixedDebugInformation() = default;

void FixedDebugInformation::loadProperties(const rapidjson::Value& inObj) {
    const auto& obj = inObj["fixedDebugInfo"];
    if (obj.IsObject()) {
        JsonHelper::getVector2(obj, "scale", &mScale);
    }
}

void FixedDebugInformation::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const {
    rapidjson::Value props(rapidjson::kObjectType);
    JsonHelper::setVector2(alloc, &props, "scale", mScale);

    inObj.AddMember("fixedDebugInfo", props, alloc);
}

void FixedDebugInformation::initialize(const IFpsGetter* getter) {
    mFpsGetter = getter;
    mFPSPos = Vector2(Window::width() / 2.f, Window::height());
}

void FixedDebugInformation::draw(DrawString& drawString) const {
    auto drawPos = mFPSPos;
    std::string str = "fps";
    drawString.drawString(str, drawPos, mScale);
    drawPos.x += DrawString::WIDTH * (str.length() + 1) * mScale.x;
    drawString.drawString(StringUtil::floatToString(mFpsGetter->getFps(), 2), drawPos, mScale);
}
