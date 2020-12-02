#include "CharacterSelector.h"
#include "../Camera/Camera.h"
#include "../Character/CharacterCommonComponents.h"
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

bool CharacterSelector::selectCharacter(
    CharacterPtr& out,
    const CharacterPtrList& characters
) const {
    //カメラからマウス位置へ向かうレイを取得する
    const auto& ray = mCamera->screenToRay(Input::mouse().getMousePosition());

    for (const auto& chara : characters) {
        //AABBとレイの衝突判定
        if (Intersect::intersectRayAABB(ray, chara->getAABBCollider().getAABB())) {
            //衝突したゲームオブジェクトを選択する
            out = chara;
            return true;
        }
    }

    //どのキャラクターとも衝突していない
    return false;
}
