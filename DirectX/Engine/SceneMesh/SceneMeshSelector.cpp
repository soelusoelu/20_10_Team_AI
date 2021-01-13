#include "SceneMeshSelector.h"
#include "../../Collision/Collision.h"
#include "../../Component/Engine/Camera/Camera.h"
#include "../../Component/Engine/Mesh/MeshComponent.h"
#include "../../GameObject/GameObject.h"
#include "../../Input/Input.h"
#include "../../System/Window.h"

SceneMeshSelector::SceneMeshSelector()
    : mCamera(nullptr)
    , mMeshesGetter(nullptr)
{
}

SceneMeshSelector::~SceneMeshSelector() = default;

void SceneMeshSelector::initialize(const std::shared_ptr<Camera>& camera, const IMeshesGetter* getter) {
    mCamera = camera;
    mMeshesGetter = getter;
}

bool SceneMeshSelector::selectMesh(std::shared_ptr<MeshComponent>& out) {
    if (!selectConditions()) {
        return false;
    }

    RaycastHit raycastHit{};
    if (!Intersect::intersectRayMeshes(mCamera->screenToRay(Input::mouse().getMousePosition()), *mMeshesGetter, &raycastHit)) {
        return false;
    }

    out = raycastHit.hitObject->componentManager().getComponent<MeshComponent>();

    return true;
}

bool SceneMeshSelector::selectConditions() const {
    const auto& mouse = Input::mouse();
    if (!mouse.getMouseButtonDown(MouseCode::LeftButton)) {
        return false;
    }
    const auto& mousePos = mouse.getMousePosition();
    if (mousePos.x > Window::width() || mousePos.y > Window::height()) {
        return false;
    }

    return true;
}
