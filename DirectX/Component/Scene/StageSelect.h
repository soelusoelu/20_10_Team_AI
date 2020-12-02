#pragma once

#include "Scene.h"

class StageSelect : public Scene {
public:
    StageSelect(GameObject& gameObject);
    ~StageSelect();
    virtual void update() override;

private:
    StageSelect(const StageSelect&) = delete;
    StageSelect& operator=(const StageSelect&) = delete;
};
