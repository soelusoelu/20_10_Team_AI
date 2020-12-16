#pragma once

#include "../Component.h"
#include "../../Math/Math.h"
#include "../../Mesh/IAnimation.h"
#include "../../Mesh/IMesh.h"
#include "../../Mesh/IMeshDrawer.h"
#include <memory>
#include <string>
#include <vector>

class Mesh;

class MeshComponent : public Component {
public:
    MeshComponent(GameObject& gameObject);
    virtual ~MeshComponent();
    virtual void awake() override;
    virtual void start() override;
    virtual void finalize() override;
    virtual void onEnable(bool value) override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const override;
    virtual void drawInspector() override;

    //メッシュを作成する
    void createMesh(const std::string& fileName, const std::string& directoryPath);

    //描画できる状態か
    bool isDraw() const;

    //状態
    void setActive(bool value);
    bool getActive() const;

    //メッシュインターフェースを取得する
    IMesh* getMesh() const;
    //アニメーションインターフェースを取得する
    IAnimation* getAnimation() const;
    //描画インターフェースを取得する
    const IMeshDrawer* getDrawer() const;

    //影の処理をするか
    bool handleShadow() const;

private:
    MeshComponent(const MeshComponent&) = delete;
    MeshComponent& operator=(const MeshComponent&) = delete;

protected:
    std::shared_ptr<Mesh> mMesh;
    std::string mFileName;
    std::string mDirectoryPath;
    bool mIsActive;
    bool mShadowHandle;
};
