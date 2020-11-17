#include "Scene.h"
#include "../../DebugLayer/Debug.h"

Scene::Scene(GameObject& gameObject) :
    Component(gameObject),
    mNext() {
}

Scene::~Scene() = default;

void Scene::initialize(const ValuePassMap& values) {
    mValuesFromPreviousScene = values;
}

void Scene::next(const std::string& next) {
    mNext = next;
}

const std::string& Scene::getNext() const {
    return mNext;
}

void Scene::addValuePassToNextScene(const std::string& valueName, const std::any& value) {
    mValuesPassToNextScene.emplace(valueName, value);
}

const ValuePassMap& Scene::getValuePassToNextScene() const {
    return mValuesPassToNextScene;
}

const std::any& Scene::getValueFromPreviousScene(const std::string& valueName) const {
    auto itr = mValuesFromPreviousScene.find(valueName);
    if (itr == mValuesFromPreviousScene.end()) {
        Debug::logError("Not found [" + valueName = "]");
        return nullptr;
    }

    return itr->second;
}
