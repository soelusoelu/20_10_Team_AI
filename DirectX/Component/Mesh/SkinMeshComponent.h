#pragma once

#include "../Component.h"
#include "../../Math/Math.h"
#include "../../Mesh/Motion.h"
#include <string>
#include <vector>

class MeshComponent;
class MeshShader;

class SkinMeshComponent : public Component {
public:
    SkinMeshComponent(GameObject& gameObject);
    ~SkinMeshComponent();
    virtual void start() override;
    virtual void update() override;

    //モーション名を設定する
    void setMotionName(const std::string& name, unsigned motionNo);
    //モーション番号からモーションを変更する
    void changeMotion(unsigned motionNo);
    //モーション名からモーションを変更する
    void changeMotion(const std::string& motionName);
    //現在のモーションを取得する
    const Motion& getCurrentMotion() const;
    //現在のモーションのフレーム数を取得する
    int getCurrentMotionFrame() const;
    //現在のボーン姿勢行列を取得する
    const std::vector<Matrix4>& getBoneCurrentFrameMatrix() const;

private:
    std::shared_ptr<MeshComponent> mMesh;
    std::shared_ptr<MeshShader> mMeshShader;
    std::vector<Matrix4> mCurrentBones;
    int mCurrentMotionNo;
    int mCurrentFrame;
};
