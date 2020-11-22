#pragma once

#include "../Component.h"

class MeshComponent;
class MeshShader;

class SkinMeshComponent : public Component {
public:
    SkinMeshComponent(GameObject& gameObject);
    ~SkinMeshComponent();
    virtual void start() override;
    virtual void update() override;

    //startが間に合わないとき用
    void setMeshAndShader(
        const std::shared_ptr<MeshComponent>& mesh,
        const std::shared_ptr<MeshShader>& shader
    );

private:
    std::shared_ptr<MeshComponent> mMesh;
    std::shared_ptr<MeshShader> mMeshShader;
    int mCurrentMotionNo;
    int mCurrentFrame;
};
