#include "DragAndDropCharacter.h"
#include "../ComponentManager.h"
#include "../Camera/Camera.h"
#include "../Collider/AABBCollider.h"
#include "../Mesh/MeshComponent.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectManager.h"
#include "../../Input/Input.h"
#include "../../System/Window.h"
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
    //マウスの座標を取得する
    const auto& mousePos = Input::mouse().getMousePosition();

    //カーソルがゲーム画面外なら終了
    if (mousePos.x > Window::width()
        || mousePos.y > Window::height())
    {
        return;
    }

    //カメラからマウスの位置へ向かうレイを取得
    const auto& rayCameraToMousePos = mCamera->screenToRay(mousePos);

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

void DragAndDropCharacter::moveToIntersectPoint(GameObject& target) const {
    //AABBコライダーを取得する
    auto aabbColl = target.componentManager().getComponent<AABBCollider>();
    if (!aabbColl) {
        return;
    }

    //ターゲットのトランスフォームを取得する
    auto& t = target.transform();

    //X軸を基準に移動制限を設ける
    auto movePoint = mIntersectPoint;

    //はみ出した当たり判定分補正を掛ける
    auto offset = aabbColl->getAABB().max.x - t.getPosition().x;
    if (movePoint.x + offset > 0.f) {
        movePoint.x = -offset;
    }

    //衝突点まで移動
    t.setPosition(movePoint);
}
