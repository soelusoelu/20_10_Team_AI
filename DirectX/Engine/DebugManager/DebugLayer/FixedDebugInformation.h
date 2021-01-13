#pragma once

#include "../../../Math/Math.h"
#include "../../../System/FpsCounter/IFpsGetter.h"
#include <rapidjson/document.h>

class DrawString;

class FixedDebugInformation {
public:
    FixedDebugInformation();
    ~FixedDebugInformation();
    void loadProperties(const rapidjson::Value& inObj);
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const;
    void initialize(const IFpsGetter* getter);
    void draw(DrawString& drawString) const;

private:
    const IFpsGetter* mFpsGetter;
    //文字のスケール
    Vector2 mScale;
    //FPSを表示する位置 固定
    Vector2 mFPSPos;
};
