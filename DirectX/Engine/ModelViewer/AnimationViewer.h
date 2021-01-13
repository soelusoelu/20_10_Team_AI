#pragma once

#include <memory>

class GameObject;
class SkinMeshComponent;

class AnimationViewer {
public:
    AnimationViewer();
    ~AnimationViewer();
    //毎フレーム更新
    void update();
    //モデル変更時
    void onChangeModel(const GameObject& newModel);
    //姿勢をTポーズにする
    void setTPose();

private:
    AnimationViewer(const AnimationViewer&) = delete;
    AnimationViewer& operator=(const AnimationViewer&) = delete;

    //モーションを変更するか
    bool isChangeMotion(int& nextMotionNumber) const;

private:
    std::shared_ptr<SkinMeshComponent> mAnimation;
};
