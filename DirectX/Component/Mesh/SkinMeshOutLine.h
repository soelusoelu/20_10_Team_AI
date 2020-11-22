#pragma once

#include "MeshOutLine.h"

class SkinMeshOutLine : public MeshOutLine {
public:
    SkinMeshOutLine(GameObject& gameObject);
    ~SkinMeshOutLine();
    virtual void awake() override;
    virtual void draw(const Camera& camera, const DirectionalLight& dirLight) const override;

private:
    SkinMeshOutLine(const SkinMeshOutLine&) = delete;
    SkinMeshOutLine& operator=(const SkinMeshOutLine&) = delete;

    //アウトラインを描画する
    void drawOutLine(const Camera& camera, const DirectionalLight& dirLight) const;
};
