#pragma once

#include "MeshComponent.h"
#include "../../Math/Math.h"

class SkinMeshComponent : public MeshComponent {
public:
    SkinMeshComponent(GameObject& gameObject);
    ~SkinMeshComponent();
    virtual void update() override;
    virtual void draw(const Camera& camera, const DirectionalLight& dirLight) const override;
};
