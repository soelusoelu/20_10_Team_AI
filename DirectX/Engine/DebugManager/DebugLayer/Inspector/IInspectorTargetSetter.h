#pragma once

#include <memory>

class GameObject;

class IInspectorTargetSetter {
public:
    virtual ~IInspectorTargetSetter() = default;
    virtual void setTarget(const std::shared_ptr<GameObject>& target) = 0;
};
