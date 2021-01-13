#include "DebugLayer.h"
#include "FixedDebugInformation.h"
#include "Hierarchy.h"
#include "Inspector/ImGuiInspector.h"

DebugLayer::DebugLayer()
    : mFixedDebugInfo(std::make_unique<FixedDebugInformation>())
    , mHierarchy(std::make_unique<Hierarchy>())
    , mInspector(std::make_unique<ImGuiInspector>())
{
}

DebugLayer::~DebugLayer() = default;

void DebugLayer::loadProperties(const rapidjson::Value& inObj) {
    mFixedDebugInfo->loadProperties(inObj);
    mHierarchy->loadProperties(inObj);
    mInspector->loadProperties(inObj);
}

void DebugLayer::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) {
    mFixedDebugInfo->saveProperties(alloc, inObj);
    mHierarchy->saveProperties(alloc, inObj);
    mInspector->saveProperties(alloc, inObj);
}

void DebugLayer::initialize(const IGameObjectsGetter* gameObjectsGetter, const IFpsGetter* fpsGetter) {
    mFixedDebugInfo->initialize(fpsGetter);
    mHierarchy->initialize(gameObjectsGetter, mInspector.get());
}

void DebugLayer::update() {
    mHierarchy->update();
}

void DebugLayer::draw(EngineMode mode, DrawString& drawer, Matrix4& proj) const {
    if (mode == EngineMode::GAME) {
        mFixedDebugInfo->draw(drawer);
    }

    mHierarchy->drawGameObjects(drawer);
    mInspector->drawInspect();
}

FixedDebugInformation& DebugLayer::fixedDebugInfo() const {
    return *mFixedDebugInfo;
}

Hierarchy& DebugLayer::hierarchy() const {
    return *mHierarchy;
}

IInspectorTargetSetter* DebugLayer::inspector() const {
    return mInspector.get();
}
