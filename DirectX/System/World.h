#pragma once

#include <memory>

class AssetsManager;

//グローバルアクセス専用クラス
class World {
private:
    World();
public:
    ~World();
    static World& instance();
    void finalize();
    AssetsManager& assetsManager() const;

private:
    World(const World&) = delete;
    World& operator=(const World&) = delete;

private:
    static inline World* mInstance = nullptr;
    std::unique_ptr<AssetsManager> mAssetsManager;
};
