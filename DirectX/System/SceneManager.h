#pragma once

#include "FpsCounter/IFpsGetter.h"
#include "../Engine/EngineMode.h"
#include "../Engine/ICallbackChangeEngineMode.h"
#include "../Engine/IEngineModeGetter.h"
#include <rapidjson/document.h>
#include <memory>
#include <string>
#include <unordered_set>

class Scene;
class Renderer;
class Camera;
class EngineFunctionManager;
class GameObjectManager;
class MeshManager;
class Physics;
class SpriteManager;
class LightManager;
class MeshRenderOnTextureManager;
class DrawString;

class SceneManager : public ICallbackChangeEngineMode, public IEngineModeGetter {
public:
    SceneManager();
    ~SceneManager();
    void loadProperties(const rapidjson::Value& inObj);
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const;
    void initialize(const IFpsGetter* fpsGetter);
    void update();
    void draw() const;

private:
    virtual void onChangeGameMode() override;
    virtual void onChangeMapEditorMode() override;
    virtual void onChangeModelViewerMode() override;

    virtual EngineMode getMode() const override;

    //シーン変更時
    void change();
    //シーン生成
    void createScene(const std::string& name);
    //最初のシーンを選択する
    void choiceBeginScene();
    //ゲーム中か
    bool isGameMode() const;

private:
    std::unique_ptr<Renderer> mRenderer;
    std::shared_ptr<Scene> mCurrentScene;
    std::shared_ptr<Camera> mCamera;
    std::unique_ptr<EngineFunctionManager> mEngineManager;
    std::unique_ptr<GameObjectManager> mGameObjectManager;
    std::unique_ptr<MeshManager> mMeshManager;
    std::unique_ptr<SpriteManager> mSpriteManager;
    std::unique_ptr<Physics> mPhysics;
    std::unique_ptr<LightManager> mLightManager;
    std::unique_ptr<MeshRenderOnTextureManager> mMeshRenderOnTextureManager;
    DrawString* mTextDrawer;
    std::string mBeginScene;
    std::string mReleaseScene;
    std::unordered_set<std::string> mRemoveExclusionTags;
    EngineMode mMode;
};
