#include "CharacterCost.h"
#include "../../Device/Subject.h"
#include "../../Imgui/imgui.h"
#include "../../Math/Math.h"
#include "../../Utility/LevelLoader.h"

CharacterCost::CharacterCost(GameObject& gameObject)
    : Component(gameObject)
    , mMaxCost(0)
    , mCurrentCost(0)
    , mSubject(std::make_unique<Subject>())
{
}

CharacterCost::~CharacterCost() = default;

void CharacterCost::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getInt(inObj, "maxCost", &mMaxCost);
    mCurrentCost = mMaxCost;
}

void CharacterCost::drawInspector() {
    ImGui::SliderInt("MaxCost", &mMaxCost, 0, 100);
    ImGui::SliderInt("CurrentCost", &mCurrentCost, 0, 100);
}

void CharacterCost::useCost(int amount) {
    mCurrentCost -= amount;
    clampCost();
    mSubject->notify();
}

void CharacterCost::returnCost(int amount) {
    mCurrentCost += amount;
    clampCost();
    mSubject->notify();
}

void CharacterCost::setCost(int cost, bool maxToo) {
    mCurrentCost = cost;
    if (maxToo) {
        mMaxCost = cost;
    }
    mSubject->notify();
}

int CharacterCost::getCost() const {
    return mCurrentCost;
}

int CharacterCost::getMaxCost() const {
    return mMaxCost;
}

void CharacterCost::callbackUpdateCost(const std::function<void()>& callback) {
    mSubject->addObserver(callback);
}

void CharacterCost::clampCost() {
    mCurrentCost = Math::clamp<int>(mCurrentCost, 0, mMaxCost);
}
