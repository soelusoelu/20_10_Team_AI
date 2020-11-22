#include "SkinMeshOutLine.h"
#include "../Camera/Camera.h"
#include "../Light/DirectionalLight.h"
#include "../../DirectX/DirectXInclude.h"
#include "../../Mesh/Material.h"
#include "../../Mesh/Mesh.h"
#include "../../System/AssetsManager.h"
#include "../../System/Shader/ConstantBuffers.h"
#include "../../System/Shader/Shader.h"
#include "../../System/Texture/TextureFromFile.h"
#include "../../Transform/Transform3D.h"
#include "../../Utility/LevelLoader.h"

SkinMeshOutLine::SkinMeshOutLine(GameObject& gameObject)
    : MeshOutLine(gameObject)
{
}

SkinMeshOutLine::~SkinMeshOutLine() = default;

void SkinMeshOutLine::awake() {
    mOutLineShader = AssetsManager::instance().createShader("OutLine.hlsl");

    MeshComponent::awake();
}

void SkinMeshOutLine::draw(const Camera& camera, const DirectionalLight& dirLight) const {
    if (mIsDrawOutLine) {
        //裏面のみ描画したいから
        MyDirectX::DirectX::instance().rasterizerState()->setCulling(CullMode::FRONT);

        //アウトライン描画
        drawOutLine(camera, dirLight);

        //設定を戻す
        MyDirectX::DirectX::instance().rasterizerState()->setCulling(CullMode::BACK);
    }

    //メッシュの描画は上層クラスに任せる
    MeshComponent::draw(camera, dirLight);
}

void SkinMeshOutLine::drawOutLine(const Camera& camera, const DirectionalLight& dirLight) const {
    //アウトラインシェーダーの登録
    //mOutLineShader->setShaderInfo();

    ////スケールを拡大したモデルをアウトラインとして描画するため
    ////ワールド行列の再計算をする
    //const auto& t = transform();
    //auto pivot = t.getPivot();
    //auto world = Matrix4::createTranslation(-pivot);
    //world *= Matrix4::createScale(t.getScale() + Vector3::one * mOutLineThickness);
    //world *= Matrix4::createFromQuaternion(t.getRotation());
    //world *= Matrix4::createTranslation(t.getPosition());

    ////シェーダーのコンスタントバッファーに各種データを渡す
    //MeshCommonConstantBuffer meshcb;
    //meshcb.world = world;
    //meshcb.view = camera.getView();
    //meshcb.projection = camera.getProjection();
    //meshcb.wvp = world * camera.getViewProjection();
    //meshcb.lightDir = dirLight.getDirection();
    //meshcb.lightColor = dirLight.getLightColor();
    //meshcb.cameraPos = camera.getPosition();
    //mOutLineShader->transferData(&meshcb, sizeof(meshcb), 0);

    //OutLineConstantBuffer outlinecb;
    //outlinecb.outlineColor = Vector4(mOutLineColor, 1.f);
    //mOutLineShader->transferData(&outlinecb, sizeof(outlinecb), 1);

    //SkinMeshConstantBuffer skinMeshcb;
    //auto& motion = mMesh->getMotion(0);
    //for (size_t i = 0; i < mMesh->getBoneCount(); i++) {
    //    skinMeshcb.bones[i] = mMesh->getBone(i).offsetMat * motion.frameMat[i][mCurrentFrame];
    //}
    //mOutLineShader->transferData(&skinMeshcb, sizeof(skinMeshcb), 2);

    ////アウトラインを描画する
    //for (size_t i = 0; i < mMesh->getMeshCount(); ++i) {
    //    mMesh->draw(i);
    //}
}
