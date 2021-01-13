#include "DebugManager.h"
#include "DebugLayer/DebugLayer.h"
#include "DebugUtility/DebugUtility.h"
#include "../../Device/DrawString.h"

DebugManager::DebugManager()
    : mStringDrawer(std::make_unique<DrawString>())
    , mDebugLayer(std::make_unique<DebugLayer>())
{
}

DebugManager::~DebugManager() {
    DebugUtility::instance().finalize();
}

void DebugManager::loadProperties(const rapidjson::Value& inObj) {
    mStringDrawer->loadProperties(inObj);
    mDebugLayer->loadProperties(inObj);
    DebugUtility::instance().loadProperties(inObj);
}

void DebugManager::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) {
    mDebugLayer->saveProperties(alloc, inObj);
    DebugUtility::instance().saveProperties(alloc, inObj);
}

void DebugManager::initialize(
    const IGameObjectsGetter* gameObjectsGetter,
    const IFpsGetter* fpsGetter,
    const IPause* pause
) {
    mStringDrawer->initialize();
    mDebugLayer->initialize(gameObjectsGetter, fpsGetter);
    DebugUtility::instance().initialize(pause);
}

void DebugManager::preUpdateProcess() {
    //描画済み文字列を削除する
    mStringDrawer->clear();
    DebugUtility::instance().preUpdateProcess();
}

void DebugManager::update() {
    mDebugLayer->update();
}

void DebugManager::draw(
    EngineMode mode,
    const Renderer& renderer,
    Matrix4& proj
) const {
    mDebugLayer->draw(mode, *mStringDrawer, proj);
    DebugUtility::instance().draw(mode, *mStringDrawer);
    mStringDrawer->drawAll(proj);
    DebugUtility::instance().drawLine2D(mode, renderer, proj);
}

void DebugManager::draw3D(
    EngineMode mode,
    const Renderer& renderer,
    const Matrix4& viewProj
) const {
    DebugUtility::instance().draw3D(mode, renderer, viewProj);
}

DebugLayer& DebugManager::getDebugLayer() const {
    return *mDebugLayer;
}
