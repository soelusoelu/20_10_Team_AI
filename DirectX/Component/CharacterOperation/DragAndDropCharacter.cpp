#include "DragAndDropCharacter.h"
#include "../Camera/Camera.h"
#include "../Collider/AABBCollider.h"
#include "../Mesh/MeshComponent.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectManager.h"
#include "../../Input/Input.h"
#include "../../Transform/Transform3D.h"

DragAndDropCharacter::DragAndDropCharacter(GameObject& gameObject)
    : Component(gameObject)
    , mCamera(nullptr)
    , mIntersectPoint(Vector3::zero)
{
}

DragAndDropCharacter::~DragAndDropCharacter() = default;

void DragAndDropCharacter::start() {
    const auto& gameObjectManager = gameObject().getGameObjectManager();
    mCamera = gameObjectManager.find("Camera")->componentManager().getComponent<Camera>();

    //指定のタグを含んでいるオブジェクトをすべて取得する
    const auto& grounds = gameObjectManager.findGameObjects("Ground");
    //取得したオブジェクトからメッシュを取得する
    for (const auto& g : grounds) {
        auto mesh = g->componentManager().getComponent<MeshComponent>();
        if (mesh) {
            mGroundMeshes.emplace_back(mesh);
        }
    }
}

void DragAndDropCharacter::dragMove(GameObject& target) {
    //マウスインターフェイスを取得
    const auto& mouse = Input::mouse();

    //マウスの左ボタンを押していなかったら終了
    if (!(mouse.getMouseButton(MouseCode::LeftButton))
        && !(mouse.getMouseButtonDown(MouseCode::LeftButton)))
    {
        return;
    }

    //カメラからマウスの位置へ向かうレイを取得
    const auto& rayCameraToMousePos = mCamera->screenToRay(mouse.getMousePosition());

    //地形とレイが衝突していなかったら終了
    if (!intersectRayGroundMeshes(rayCameraToMousePos)) {
        return;
    }

    //条件を満たしていたら移動
    moveToIntersectPoint(target);
}

bool DragAndDropCharacter::intersectRayGroundMeshes(const Ray& ray) {
    //すべての地形メッシュとレイの衝突判定
    for (const auto& gm : mGroundMeshes) {
        if (Intersect::intersectRayMesh(ray, gm->getMesh(), gm->transform(), mIntersectPoint)) {
            return true;
        }
    }

    //どれとも衝突しなかった
    return false;
}

void DragAndDropCharacter::moveToIntersectPoint(GameObject& target) {
    //X軸を基準に移動制限を設ける
    auto movePoint = mIntersectPoint;
    //auto offset = mAABB->getAABB().max.x - transform().getPosition().x;
    //if (movePoint.x + offset > 0.f) {
    //    movePoint.x = -offset;
    //}

    //衝突点まで移動
    target.transform().setPosition(movePoint);
}
