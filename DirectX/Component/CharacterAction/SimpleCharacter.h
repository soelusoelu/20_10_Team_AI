#pragma once

#include "../Component.h"
#include <memory>

class SkinMeshComponent;

class SimpleCharacter : public Component {
public:
    SimpleCharacter(GameObject& gameObject);
    ~SimpleCharacter();
    virtual void start() override;
    //アクションフェーズ移行時の処理
    void onChangeActionPhase();
    //操作フェーズ移行時の処理
    void onChangeOperatePhase();

private:
    SimpleCharacter(const SimpleCharacter&) = delete;
    SimpleCharacter& operator=(const SimpleCharacter&) = delete;

private:
    std::shared_ptr<SkinMeshComponent> mSkinMesh;
};
