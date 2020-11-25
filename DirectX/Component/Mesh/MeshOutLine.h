#pragma once

#include "MeshComponent.h"
#include "../../Math/Math.h"
#include <memory>

class Shader;
class SkinMeshComponent;

class MeshOutLine : public MeshComponent {
public:
    MeshOutLine(GameObject& gameObject);
    ~MeshOutLine();
    virtual void awake() override;
    virtual void start() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const override;
    virtual void drawInspector() override;
    virtual void draw(const Camera& camera, const DirectionalLight& dirLight) const override;

    //アウトラインの色を設定する
    void setOutLineColor(const Vector3& color);
    //アウトラインの色を取得する
    const Vector3& getOutLineColor() const;
    //アウトラインの太さを設定する
    void setOutLineThickness(float thickness);
    //アウトラインの太さを取得する
    float getOutLineThickness() const;
    //アウトラインを描画するか設定する
    void setActiveOutLine(bool value);
    //アウトラインを描画するか取得する
    bool getActiveOutLine() const;

private:
    MeshOutLine(const MeshOutLine&) = delete;
    MeshOutLine& operator=(const MeshOutLine&) = delete;

    //アウトラインを描画する
    void drawOutLine(const Camera& camera, const DirectionalLight& dirLight) const;

protected:
    std::shared_ptr<Shader> mOutLineShader;
    std::weak_ptr<SkinMeshComponent> mSkinMesh;
    Vector3 mOutLineColor;
    float mOutLineThickness;
    bool mIsDrawOutLine;
    bool mIsAnimation;
    float mOffset;
};
