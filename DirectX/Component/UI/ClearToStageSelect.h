#pragma once

#include "../Component.h"
#include <memory>

class Scene;
class SpriteButtonComponent;

class ClearToStageSelect : public Component {
public:
    ClearToStageSelect(GameObject& gameObject);
    ~ClearToStageSelect();
    virtual void start() override;

    //ステージクリア時
    void onStageClear();

private:
    ClearToStageSelect(const ClearToStageSelect&) = delete;
    ClearToStageSelect& operator=(const ClearToStageSelect&) = delete;

private:
    std::weak_ptr<Scene> mCurrentScene;
    std::shared_ptr<SpriteButtonComponent> mButton;
};
