#pragma once

#include "../../EngineMode.h"
#include "../../Pause/IPause.h"
#include "../../../Math/Math.h"
#include <rapidjson/document.h>
#include <memory>

class DrawString;
class Renderer;
class Log;
class PointRenderer;
class LineRenderer2D;
class LineRenderer3D;

class DebugUtility {
private:
    DebugUtility();

public:
    ~DebugUtility();
    static DebugUtility& instance();
    void loadProperties(const rapidjson::Value& inObj);
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj);
    void initialize(const IPause* pause);
    void finalize();
    void preUpdateProcess();
    void draw(EngineMode mode, DrawString& drawer) const;
    void drawLine2D(EngineMode mode, const Renderer& renderer, Matrix4& proj) const;
    void draw3D(EngineMode mode, const Renderer& renderer, const Matrix4& viewProj) const;
    Log& log() const;
    PointRenderer& pointRenderer() const;
    LineRenderer2D& lineRenderer2D() const;
    LineRenderer3D& lineRenderer3D() const;

private:
    DebugUtility(const DebugUtility&) = delete;
    DebugUtility& operator=(const DebugUtility&) = delete;

private:
    static inline DebugUtility* mInstance = nullptr;

    const IPause* mPause;
    std::unique_ptr<Log> mLog;
    std::unique_ptr<PointRenderer> mPointRenderer;
    std::unique_ptr<LineRenderer2D> mLineRenderer2D;
    std::unique_ptr<LineRenderer3D> mLineRenderer3D;
};
