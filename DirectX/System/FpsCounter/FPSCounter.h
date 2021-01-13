#pragma once

#include "IFpsGetter.h"
#include "../SystemInclude.h"
#include <rapidjson/document.h>
#include <memory>

class Time;

class FPSCounter : public IFpsGetter {
public:
    FPSCounter();
    ~FPSCounter();
    virtual float getFps() const override;
    void loadProperties(const rapidjson::Value& inObj);
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const;
    //設定したフレームレートに固定する
    void fixedFrame();
    //固定フレーム値を設定する
    void setFixedFrame(float fixedFrame);

private:
    //フレームレートを計算する
    void computeFps(float time);

private:
    std::unique_ptr<Time> mDrawUpdateTimer;
    float mFixedFrame;
    float mCurrentFPS;
    LARGE_INTEGER mFrequency;
    unsigned long long mPreviousTime;
};
