#include "SceneMeshDeleter.h"
#include "../../Component/Engine/Mesh/MeshComponent.h"
#include "../../GameObject/GameObject.h"
#include "../../Input/Input.h"

SceneMeshDeleter::SceneMeshDeleter() = default;

SceneMeshDeleter::~SceneMeshDeleter() = default;

void SceneMeshDeleter::deleteMesh(std::shared_ptr<MeshComponent>& target) {
    if (!target) {
        return;
    }
    if (!Input::keyboard().getKeyDown(KeyCode::Delete)) {
        return;
    }

    target->gameObject().destroy();
    target.reset();
}
