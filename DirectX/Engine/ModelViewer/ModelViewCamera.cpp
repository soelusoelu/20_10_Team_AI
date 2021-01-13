#include "ModelViewCamera.h"
#include "../Camera/SimpleCamera.h"
#include "../../Collision/Collision.h"
#include "../../Component/Engine/Camera/CameraHelper.h"
#include "../../Input/Input.h"
#include "../../System/Window.h"

ModelViewCamera::ModelViewCamera()
    : mCamera(std::make_unique<SimpleCamera>())
    , mModelCenterView()
{
}

ModelViewCamera::~ModelViewCamera() = default;

void ModelViewCamera::update() {
    if (Input::keyboard().getKeyDown(KeyCode::F)) {
        setModelCenterPosition();
    }
}

void ModelViewCamera::onChangeModel(const IMesh& mesh) {
    //モデルを包む球を求める
    Sphere sphere;
    SphereHelper::create(sphere, mesh);

    //モデル全体を映すビュー行列を求める
    const auto& view = CameraHelper::getViewMatrixTakingSphereInCamera(
        sphere,
        Window::width(),
        Window::height(),
        FOV,
        Vector3::forward,
        Vector3::up
    );
    //カメラにビュー行列を設定する
    mCamera->setView(view);
    //モデル全体を映すビュー行列を保存する
    mModelCenterView = view;

    //射影行列を求め、カメラに設定する
    mCamera->setProjection(Matrix4::createPerspectiveFOV(
        Window::width(),
        Window::height(),
        FOV,
        NEAR_CLIP,
        FAR_CLIP
    ));
}

void ModelViewCamera::setModelCenterPosition() {
    mCamera->setView(mModelCenterView);
}

const SimpleCamera& ModelViewCamera::getCamera() const {
    return *mCamera;
}
