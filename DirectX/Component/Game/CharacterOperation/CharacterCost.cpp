#include "CharacterCost.h"
#include "../../../Device/Subject.h"
#include "../../../Engine/DebugManager/DebugLayer/Inspector/ImGuiWrapper.h"
#include "../../../Math/Math.h"
#include "../../../Utility/LevelLoader.h"

CharacterCost::CharacterCost(GameObject& gameObject)
    : Component(gameObject)
    , mMaxCost(0)
    , mCurrentCost(0)
    , mCallbackUpdateCost(std::make_unique<Subject>())
{
}

CharacterCost::~CharacterCost() = default;

void CharacterCost::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getInt(inObj, "maxCost", &mMaxCost);
    mCurrentCost = mMaxCost;
}

void CharacterCost::drawInspector() {
    if (ImGuiWrapper::sliderInt("CurrentCost", mCurrentCost, 0, mMaxCost)) {
        mCallbackUpdateCost->notify();
    }
    if (ImGuiWrapper::dragInt("MaxCost", mMaxCost, 1.f, 0)) {
        mCallbackUpdateCost->notify();
    }
}

void CharacterCost::useCost(int amount) {
    mCurrentCost -= amount;
    clampCost();
    mCallbackUpdateCost->notify();
}

void CharacterCost::returnCost(int amount) {
    mCurrentCost += amount;
    clampCost();
    mCallbackUpdateCost->notify();
}

void CharacterCost::setCost(int cost, bool maxToo) {
    mCurrentCost = cost;
    if (maxToo) {
        mMaxCost = cost;
    }
    mCallbackUpdateCost->notify();
}

int CharacterCost::getCost() const {
    return mCurrentCost;
}

int CharacterCost::getMaxCost() const {
    return mMaxCost;
}

void CharacterCost::callbackUpdateCost(const std::function<void()>& callback) {
    mCallbackUpdateCost->addObserver(callback);
}

void CharacterCost::clampCost() {
    mCurrentCost = Math::clamp<int>(mCurrentCost, 0, mMaxCost);
}
