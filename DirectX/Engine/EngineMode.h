#pragma once

enum class EngineMode {
    GAME,
    MAP_EDITOR,
    MODEL_VIEWER
};

namespace EngineModeName {
constexpr char MAP_EDITOR[] = "MapEditor";
constexpr char MODEL_VIEWER[] = "ModelViewer";
}
