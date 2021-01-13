#pragma once

#include "IInspectorTargetSetter.h"
#include "../../../../Math/Math.h"
#include <rapidjson/document.h>
#include <any>
#include <memory>
#include <string>

class Component;
class GameObject;
class Transform3D;

//ImGuiを使用したインスペクター
class ImGuiInspector : public IInspectorTargetSetter {
public:
    ImGuiInspector();
    ~ImGuiInspector();
    virtual void setTarget(const std::shared_ptr<GameObject>& target) override;
    void loadProperties(const rapidjson::Value& inObj);
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const;
    void drawInspect() const;

private:
    ImGuiInspector(const ImGuiInspector&) = delete;
    ImGuiInspector& operator=(const ImGuiInspector&) = delete;

    void drawName(const GameObject& target) const;
    void drawTag(const GameObject& target) const;

private:
    std::weak_ptr<GameObject> mTarget;
    float mInspectorPositionX;
};
