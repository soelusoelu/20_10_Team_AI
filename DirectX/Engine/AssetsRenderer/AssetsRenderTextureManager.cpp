#include "AssetsRenderTextureManager.h"
#include "AssetsRenderTextureAdder.h"
#include "AssetsRenderTextureDeleter.h"
#include "AssetsRenderTextureList.h"
#include "AssetsTexturesSelector.h"

AssetsRenderTextureManager::AssetsRenderTextureManager()
    : mTextureList(std::make_unique<AssetsRenderTextureList>())
    , mTextureAdder(std::make_unique<AssetsRenderTextureAdder>())
    , mDeleter(std::make_unique<AssetsRenderTextureDeleter>())
    , mSelector(std::make_unique<AssetsTexturesSelector>())
    , mCurrentSelectTexture(nullptr)
{
}

AssetsRenderTextureManager::~AssetsRenderTextureManager() = default;

const AssetsRenderTexture& AssetsRenderTextureManager::getCurrentSelectTexture() const {
    return *mCurrentSelectTexture;
}

bool AssetsRenderTextureManager::selectedTexture() const {
    return (mCurrentSelectTexture) ? true : false;
}

void AssetsRenderTextureManager::loadProperties(const rapidjson::Value& inObj) {
    mTextureList->loadProperties(inObj);
    mTextureAdder->loadProperties(inObj);
}

void AssetsRenderTextureManager::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const {
    mTextureList->saveProperties(alloc, inObj);
    mTextureAdder->saveProperties(alloc, inObj);
}

void AssetsRenderTextureManager::initialize() {
    mTextureList->initialize();
    mTextureAdder->initialize(mTextureList.get());
    mDeleter->initialize(mTextureList.get(), this);
    mSelector->initialize(mTextureList.get());
}

void AssetsRenderTextureManager::update(EngineMode mode) {
    if (isProcessMode(mode)) {
        mSelector->selectTexture(mCurrentSelectTexture);
        mTextureAdder->update();
        mDeleter->update();
    }
}

void AssetsRenderTextureManager::drawMeshes(EngineMode mode) const {
    if (isProcessMode(mode)) {
        mTextureList->drawMeshOnTexture();
    }
}

void AssetsRenderTextureManager::drawTextures(EngineMode mode, const Matrix4& proj) const {
    if (isProcessMode(mode)) {
        mTextureList->drawTexture(proj);
        mTextureAdder->draw(proj);
    }
}

ICallbackSelectAssetsTexture* AssetsRenderTextureManager::getCallbackSelectAssetsTexture() const {
    return mSelector.get();
}

bool AssetsRenderTextureManager::isProcessMode(EngineMode mode) const {
    if (mode == EngineMode::MAP_EDITOR) {
        return true;
    }
    if (mode == EngineMode::MODEL_VIEWER) {
        return true;
    }

    return false;
}
