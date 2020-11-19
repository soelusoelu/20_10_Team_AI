#pragma once

#include "../Component.h"
#include "../../Collision/Collision.h"
#include "../../Math/Math.h"
#include <memory>
#include <vector>

class Camera;
class MeshComponent;

//キャラクターをマウスで操作するクラス
class DragAndDropCharacter : public Component {
public:
    DragAndDropCharacter(GameObject& gameObject);
    ~DragAndDropCharacter();
    virtual void start() override;
    void dragMove(GameObject& target);

private:
    DragAndDropCharacter(const DragAndDropCharacter&) = delete;
    DragAndDropCharacter& operator=(const DragAndDropCharacter&) = delete;

    //すべての地形メッシュとレイの衝突判定を行う
    bool intersectRayGroundMeshes(const Ray& ray);
    //メッシュを衝突点まで移動させる
    void moveToIntersectPoint(GameObject& target);

private:
    std::shared_ptr<Camera> mCamera;
    //キャラクターを立たせたい地形メッシュ配列
    std::vector<std::shared_ptr<MeshComponent>> mGroundMeshes;
    //レイと地形との衝突点
    Vector3 mIntersectPoint;
};
