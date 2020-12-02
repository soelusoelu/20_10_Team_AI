#include "DragAndDropCharacter.h"
#include "../Character/CharacterCommonComponents.h"
#include "../Character/OverlapPrevention.h"
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
    , mOverlapPreventor(nullptr)
    , mManager(nullptr)
    , mIntersectPoint(Vector3::zero)
{
}

DragAndDropCharacter::~DragAndDropCharacter() = default;

void DragAndDropCharacter::start() {
    const auto& gameObjectManager = gameObject().getGameObjectManager();
    mCamera = gameObjectManager.find("Camera")->componentManager().getComponent<Camera>();

    mOverlapPreventor = getComponent<OverlapPrevention>();
}

void DragAndDropCharacter::dragMove(const CharacterCommonComponents& target) {
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

    const auto& t = target.transform();
    //移動前の位置を確保
    const auto& prePos = t.getPosition();

    //条件を満たしていたら移動
    moveToIntersectPoint(target);

    //押し出し処理
    mOverlapPreventor->overlapPrevent(
        target.getAABBCollider(),
        target.getManager().getCharacters(),
        t.getPosition(),
        prePos
    );
}

void DragAndDropCharacter::setManager(const ICharacterManager* manager) {
    mManager = manager;
}

bool DragAndDropCharacter::intersectRayGroundMeshes(const Ray& ray) {
    //地形メッシュとレイの衝突判定
    const auto& map = mManager->getMap();
    return Intersect::intersectRayMesh(ray, map.getMeshData(), map.getTransform(), &mIntersectPoint);
}

void DragAndDropCharacter::moveToIntersectPoint(const CharacterCommonComponents& target) const {
    //ターゲットのトランスフォームを取得する
    auto& t = target.transform();

    const auto& aabb = target.getAABBCollider().getAABB();

    //X軸を基準に移動制限を設ける
    auto movePoint = mIntersectPoint;

    //はみ出した当たり判定分補正を掛ける
    auto offset = aabb.max.x - t.getPosition().x;
    if (movePoint.x + offset > 0.f) {
        movePoint.x = -offset;
    }

    //衝突点まで移動
    t.setPosition(movePoint);
}
