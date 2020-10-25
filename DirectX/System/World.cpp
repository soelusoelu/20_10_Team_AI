#include "World.h"
#include "../Device/AssetsManager.h"
#include "../System/GlobalFunction.h"

World::World() :
    mAssetsManager(std::make_unique<AssetsManager>()) {
}

World::~World() = default;

World& World::instance() {
    if (!mInstance) {
        mInstance = new World();
    }
    return *mInstance;
}

void World::finalize() {
    safeDelete(mInstance);
}

AssetsManager& World::assetsManager() const {
    return *mAssetsManager;
}
