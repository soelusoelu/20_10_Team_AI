#pragma once

#include "EngineMode.h"

class IEngineModeGetter {
public:
    virtual ~IEngineModeGetter() = default;
    virtual EngineMode getMode() const = 0;
};
