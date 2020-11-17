#pragma once

#include "../Component.h"
#include <memory>

class Scene;

class StageSelect : public Component {
public:
    StageSelect(GameObject& gameObject);
    ~StageSelect();
    virtual void start() override;
    virtual void update() override;

private:
    StageSelect(const StageSelect&) = delete;
    StageSelect& operator=(const StageSelect&) = delete;

private:
    std::shared_ptr<Scene> mScene;
};
