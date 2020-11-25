#pragma once

#include "IChangeActionPhase.h"
#include "../Component.h"
#include <memory>

class SkinMeshComponent;

class SimpleCharacter : public Component, public IChangeActionPhase {
public:
    SimpleCharacter(GameObject& gameObject);
    ~SimpleCharacter();
    virtual void start() override;

private:
    SimpleCharacter(const SimpleCharacter&) = delete;
    SimpleCharacter& operator=(const SimpleCharacter&) = delete;

    virtual void onChangeActionPhase() override;

private:
    std::shared_ptr<SkinMeshComponent> mSkinMesh;
};
