#include "CharacterSelector.h"
#include "../ComponentManager.h"
#include "../Camera/Camera.h"
#include "../Collider/AABBCollider.h"
#include "../../Collision/Collision.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectManager.h"
#include "../../Input/Input.h"

CharacterSelector::CharacterSelector(GameObject& gameObject)
    : Component(gameObject)
{
}

CharacterSelector::~CharacterSelector() = default;

void CharacterSelector::start() {
    const auto& gameObjectManager = gameObject().getGameObjectManager();
    mCamera = gameObjectManager.find("Camera")->componentManager().getComponent<Camera>();
}

void CharacterSelector::selectGameObject(GameObjectPtr& out, const GameObjectPtrList& characters) {
    //マウスインターフェイスを取得
    const auto& mouse = Input::mouse();

    //マウスの左ボタンを押した瞬間じゃなければ終了
    if (!mouse.getMouseButtonDown(MouseCode::LeftButton)) {
        return;
    }

    //カメラからマウス位置へ向かうレイを取得する
    const auto& ray = mCamera->screenToRay(mouse.getMousePosition());
    //レイからAABBを作成
    AABB rayAABB;
    rayAABB.updateMinMax(ray.start);
    rayAABB.updateMinMax(ray.end);

    for (const auto& chara : characters) {
        //キャラからAABBを取得
        auto charaAABB = chara->componentManager().getComponent<AABBCollider>()->getAABB();

        //レイとキャラで擬似衝突判定
        //if (!Intersect::intersectAABB(rayAABB, charaAABB)) {
        //    continue;
        //}

        //AABBとレイの衝突判定
        if (Intersect::intersectRayAABB(ray, charaAABB)) {
            //衝突したゲームオブジェクトを選択する
            out = chara;
            return;
        }
    }
}
