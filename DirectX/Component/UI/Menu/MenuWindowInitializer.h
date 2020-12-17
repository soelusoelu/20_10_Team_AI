#pragma once

#include "../../Component.h"

class MenuWindowInitializer : public Component {
public:
    MenuWindowInitializer(GameObject& gameObject);
    ~MenuWindowInitializer();
    virtual void start() override;

private:
    MenuWindowInitializer(const MenuWindowInitializer&) = delete;
    MenuWindowInitializer& operator=(const MenuWindowInitializer&) = delete;
};
