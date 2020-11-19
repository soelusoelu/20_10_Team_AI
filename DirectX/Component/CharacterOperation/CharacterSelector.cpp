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

void CharacterSelector::selectGameObject(GameObjectPtr& out, const GameObjectPtrList& characters) const {
    //カメラからマウス位置へ向かうレイを取得する
    const auto& ray = mCamera->screenToRay(Input::mouse().getMousePosition());

    for (const auto& chara : characters) {
        //キャラからAABBを取得
        auto charaAABB = chara->componentManager().getComponent<AABBCollider>()->getAABB();

        //AABBとレイの衝突判定
        if (Intersect::intersectRayAABB(ray, charaAABB)) {
            //衝突したゲームオブジェクトを選択する
            out = chara;
            return;
        }
    }
}
