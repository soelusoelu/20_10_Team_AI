#pragma once

#include "Scene.h"

class Title : public Scene {
public:
    Title(GameObject& gameObject);
    ~Title();
    virtual void awake() override;

private:
    Title(const Title&) = delete;
    Title& operator=(const Title&) = delete;
};
