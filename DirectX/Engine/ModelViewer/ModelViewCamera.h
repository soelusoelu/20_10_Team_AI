#pragma once

#include "../../Math/Math.h"
#include "../../Mesh/IMesh.h"

class EngineCamera;
class SimpleCamera;

class ModelViewCamera {
public:
    ModelViewCamera();
    ~ModelViewCamera();
    void initialize();
    void update();
    //モデル変更時
    void onChangeModel(const IMesh& mesh);
    //カメラをモデル全体が映る位置(初期位置)に移動する
    void setModelCenterPosition();
    //カメラを取得する
    const SimpleCamera& getCamera() const;

private:
    ModelViewCamera(const ModelViewCamera&) = delete;
    ModelViewCamera& operator=(const ModelViewCamera&) = delete;

private:
    std::unique_ptr<EngineCamera> mCamera;
    Matrix4 mModelCenterView;

    static constexpr inline float FOV = 45.f;
    static constexpr inline float NEAR_CLIP = 0.1f;
    static constexpr inline float FAR_CLIP = 1000.f;
};
