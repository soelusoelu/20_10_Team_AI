#include "Debug.h"
#include "DebugUtility.h"
#include "LineRenderer/LineRenderer2D.h"
#include "LineRenderer/LineRenderer3D.h"
#include "Log.h"
#include "PointRenderer.h"
#include "../../../System/SystemInclude.h"

void Debug::windowMessage(const std::string& message) {
#ifdef _DEBUG
    MessageBoxA(0, message.c_str(), 0, MB_OK);
#endif // _DEBUG
}

void Debug::log(const std::string& message) {
#ifdef _DEBUG
    DebugUtility::instance().log().log(message);
#endif // _DEBUG
}

void Debug::logError(const std::string& message) {
#ifdef _DEBUG
    DebugUtility::instance().log().logError(message);
#endif // _DEBUG
}

void Debug::logWarning(const std::string& message) {
#ifdef _DEBUG
    DebugUtility::instance().log().logWarning(message);
#endif // _DEBUG
}

void Debug::logClear() {
#ifdef _DEBUG
    DebugUtility::instance().log().clear();
#endif // _DEBUG
}

void Debug::renderPoint(const Vector3& point, const Vector3& color) {
#ifdef _DEBUG
    DebugUtility::instance().pointRenderer().renderPoint(point, color);
#endif // _DEBUG
}

void Debug::renderLine(const Vector2& p1, const Vector2& p2, const Vector3& color) {
#ifdef _DEBUG
    DebugUtility::instance().lineRenderer2D().renderLine(p1, p2, color);
#endif // _DEBUG
}

void Debug::renderLine(const Vector3& p1, const Vector3& p2, const Vector3& color) {
#ifdef _DEBUG
    DebugUtility::instance().lineRenderer3D().renderLine(p1, p2, color);
#endif // _DEBUG
}
