#include "SkinMeshComponent.h"
#include "../Camera/Camera.h"
#include "../Light/DirectionalLight.h"
#include "../../Mesh/Material.h"
#include "../../Mesh/Mesh.h"
#include "../../System/Shader/ConstantBuffers.h"
#include "../../System/Shader/Shader.h"
#include "../../System/Texture/TextureFromFile.h"
#include "../../Transform/Transform3D.h"

SkinMeshComponent::SkinMeshComponent(GameObject& gameObject) :
    MeshComponent(gameObject) {
}

SkinMeshComponent::~SkinMeshComponent() = default;

void SkinMeshComponent::update() {
    auto& bones = mMesh->getBones();
    //差分姿勢 * 初期姿勢（共に親空間ベース）です。
    for (int i = 0; i < mMesh->getBones().size(); ++i) {
        //bones[i].boneMat = bones[i].frameMat * bones[i].initMat;
    }
}

void SkinMeshComponent::draw(const Camera& camera, const DirectionalLight& dirLight) const {
    //使用するシェーダーの登録
    mShader->setShaderInfo();

    //シェーダーのコンスタントバッファーに各種データを渡す
    SkinMeshConstantBuffer meshcb;
    const auto& world = transform().getWorldTransform();
    auto& bones = mMesh->getBones();
    for (size_t i = 0; i < 12; i++) {
        meshcb.world[i] = world;
    }
    meshcb.view = camera.getView();
    meshcb.proj = camera.getProjection();
    mShader->transferData(&meshcb, sizeof(meshcb), 0);

    for (size_t i = 0; i < mMesh->getMeshCount(); ++i) {
        MaterialConstantBuffer matcb;
        const auto& mat = mMesh->getMaterial(i);
        matcb.ambient = mat.ambient;
        //アルファ値は0のときが多いから
        float alpha = mAlpha;
        if (!Math::nearZero(mat.transparency)) {
            alpha *= mat.transparency;
        }
        matcb.diffuse = Vector4(mat.diffuse, alpha);
        matcb.specular = mat.specular;
        //データ転送
        mShader->transferData(&matcb, sizeof(matcb), 1);

        //テクスチャが有るなら登録
        //if (mat.texture) {
        //    mat.texture->setTextureInfo();
        //}

        //描画
        mMesh->draw(i);
    }
}
