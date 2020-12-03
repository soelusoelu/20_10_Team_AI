#pragma once

#include "../Component.h"
#include "../Character/ICharacterManager.h"
#include "../../Collision/Collision.h"
#include "../../Math/Math.h"
#include <memory>

class Camera;
class CharacterCommonComponents;
class OverlapPrevention;

//キャラクターをマウスで操作するクラス
class DragAndDropCharacter : public Component {
public:
    DragAndDropCharacter(GameObject& gameObject);
    ~DragAndDropCharacter();
    virtual void start() override;

    //マウスの位置にターゲットを移動させる
    void dragMove(const CharacterCommonComponents& target) const;
    //キャラクターマネージャーを設定する
    void setManager(const ICharacterManager* manager);

private:
    DragAndDropCharacter(const DragAndDropCharacter&) = delete;
    DragAndDropCharacter& operator=(const DragAndDropCharacter&) = delete;

    //すべての地形メッシュとレイの衝突判定を行う
    bool intersectRayGroundMeshes(const Ray& ray, Vector3& intersectPoint, Triangle& intersectPolygon) const;
    //衝突した地形のポリゴンと法線による判定を行う
    bool comparePolygonNormal(const Triangle& intersectPolygon) const;
    //キャラクターを移動させる
    void moveCharacter(const CharacterCommonComponents& target, const Vector3& intersectPoint) const;
    //キャラクターを衝突点まで移動させる
    void moveToIntersectPoint(const CharacterCommonComponents& target, const Vector3& intersectPoint) const;

private:
    std::shared_ptr<Camera> mCamera;
    std::shared_ptr<OverlapPrevention> mOverlapPreventor;
    const ICharacterManager* mManager;
};
