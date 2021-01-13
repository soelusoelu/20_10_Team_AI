#pragma once

class ICallbackChangeEngineMode {
public:
    virtual ~ICallbackChangeEngineMode() = default;
    virtual void onChangeGameMode() = 0;
    virtual void onChangeMapEditorMode() = 0;
    virtual void onChangeModelViewerMode() = 0;
};
