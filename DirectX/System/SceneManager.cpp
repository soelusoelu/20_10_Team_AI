#include "SceneManager.h"
#include "Game.h"
#include "GlobalFunction.h"
#include "Texture/MeshRenderOnTextureManager.h"
#include "../Component/ComponentManager.h"
#include "../Component/Engine/Camera/Camera.h"
#include "../Component/Engine/Light/DirectionalLight.h"
#include "../Component/Engine/Scene/Scene.h"
#include "../Component/Engine/Text/TextBase.h"
#include "../Device/DrawString.h"
#include "../Device/Physics.h"
#include "../Device/Renderer.h"
#include "../Engine/EngineFunctionManager.h"
#include "../Engine/DebugManager/DebugManager.h"
#include "../Engine/DebugManager/DebugLayer/DebugLayer.h"
#include "../Engine/DebugManager/DebugLayer/Hierarchy.h"
#include "../Engine/DebugManager/DebugLayer/Inspector/ImGuiInspector.h"
#include "../Engine/MapEditor/MapEditorMeshManager.h"
#include "../Engine/Pause/Pause.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectFactory.h"
#include "../GameObject/GameObjectManager.h"
#include "../Light/LightManager.h"
#include "../Mesh/MeshManager.h"
#include "../Sprite/Sprite.h"
#include "../Sprite/SpriteManager.h"
#include "../Utility/LevelLoader.h"
#include <vector>

SceneManager::SceneManager()
    : mRenderer(std::make_unique<Renderer>())
    , mCurrentScene(nullptr)
    , mCamera(nullptr)
    , mEngineManager(std::make_unique<EngineFunctionManager>())
    , mGameObjectManager(std::make_unique<GameObjectManager>())
    , mMeshManager(std::make_unique<MeshManager>())
    , mSpriteManager(std::make_unique<SpriteManager>())
    , mPhysics(std::make_unique<Physics>())
    , mLightManager(std::make_unique<LightManager>())
    , mMeshRenderOnTextureManager(std::make_unique<MeshRenderOnTextureManager>())
    , mTextDrawer(new DrawString())
    , mBeginScene()
    , mReleaseScene()
    , mMode(EngineMode::GAME)
{
}

SceneManager::~SceneManager() {
    safeDelete(mTextDrawer);

    TextBase::setDrawString(nullptr);
}

void SceneManager::loadProperties(const rapidjson::Value& inObj) {
    const auto& sceneObj = inObj["sceneManager"];
    if (sceneObj.IsObject()) {
        JsonHelper::getString(sceneObj, "beginScene", &mBeginScene);
        JsonHelper::getString(sceneObj, "releaseScene", &mReleaseScene);
        JsonHelper::getStringArray(sceneObj, "removeExclusionTag", &mRemoveExclusionTags);
    }

    mEngineManager->loadProperties(inObj);
    mMeshManager->loadProperties(inObj);
    mLightManager->loadProperties(inObj);
    mTextDrawer->loadProperties(inObj);
}

void SceneManager::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const {
    rapidjson::Value props(rapidjson::kObjectType);
    JsonHelper::setString(alloc, &props, "beginScene", mBeginScene);
    JsonHelper::setString(alloc, &props, "releaseScene", mReleaseScene);
    JsonHelper::setStringArray(alloc, &props, "removeExclusionTag", mRemoveExclusionTags);
    inObj.AddMember("sceneManager", props, alloc);

    mEngineManager->saveProperties(alloc, inObj);
    mMeshManager->saveProperties(alloc, inObj);
    mLightManager->saveProperties(alloc, inObj);
    mTextDrawer->saveProperties(alloc, inObj);
}

void SceneManager::initialize(const IFpsGetter* fpsGetter) {
    //デフォルトカメラを作成する
    auto cam = GameObjectCreater::create("Camera");
    mCamera = cam->componentManager().getComponent<Camera>();

    mRenderer->initialize();
    mEngineManager->initialize(mCamera, this, this, mGameObjectManager.get(), mMeshManager.get(), fpsGetter);
    mMeshManager->initialize();
    mLightManager->initialize();
    mTextDrawer->initialize();

    TextBase::setDrawString(mTextDrawer);

    //デフォルトのインスペクターの対象に設定する
    mEngineManager->debug().getDebugLayer().inspector()->setTarget(cam);

    mLightManager->createDirectionalLight();

    //初期シーンの設定
#ifdef _DEBUG
    choiceBeginScene();
#else
    onChangeGameMode();
    createScene(mReleaseScene);
#endif // _DEBUG
}

void SceneManager::update() {
    //アップデート前処理
    mEngineManager->preUpdateProcess();
    //デバッグ
    mEngineManager->update(mMode);

    //ポーズ中はデバッグだけアップデートを行う
    if (mEngineManager->pause().isPausing()) {
        return;
    }

    //ゲーム中なら
    if (isGameMode()) {
        //保有しているテキストを全削除
        mTextDrawer->clear();
        //全ゲームオブジェクトの更新
        mGameObjectManager->update();
        //総当たり判定
        mPhysics->sweepAndPrune();
        //各マネージャークラスを更新
        mMeshManager->update();
        mSpriteManager->update();
        mMeshRenderOnTextureManager->update();

        //シーン移行
        const auto& next = mCurrentScene->getNext();
        if (!next.empty()) {
            change();
            //次のシーンに渡す値を避難させとく
            auto toNextValues = mCurrentScene->getValuePassToNextScene();
            //シーン遷移
            createScene(next);
            //新しいシーンに前のシーンの値を渡す
            mCurrentScene->getValueFromPreviousScene(toNextValues);
        }
    }
}

void SceneManager::draw() const {
    const auto& view = mCamera->getView();
    const auto& proj = mCamera->getProjection();

#pragma region 遅延シェーディング
    //各テクスチャ上にレンダリング
    //mRenderer->renderToTexture();
    ////メッシュ描画準備
    //mRenderer->renderMesh();
    ////メッシュの一括描画
    //mMeshManager->draw(*mCamera, mLightManager->getDirectionalLight());
    ////各テクスチャを参照してレンダリング
    //mRenderer->renderFromTexture(*mCamera, *mLightManager);
    ////ポイントライト描画準備
    //mRenderer->renderPointLight();
    ////ポイントライトの一括描画
    //mLightManager->drawPointLights(*mCamera);
#pragma endregion

    //メッシュ描画準備
    mRenderer->renderMesh();

    if (isGameMode()) {
        //メッシュの描画
        const auto& dirLight = mLightManager->getDirectionalLight();
        mMeshManager->draw(view, proj, mCamera->getPosition(), dirLight.getDirection(), dirLight.getLightColor());

        //メッシュをテクスチャに描画する
        mMeshRenderOnTextureManager->drawMeshOnTextures();
    }

#ifdef _DEBUG
    mEngineManager->draw3D(mMode, *mRenderer, *mCamera, mLightManager->getDirectionalLight());
#endif // _DEBUG

    //スプライト描画準備
    mRenderer->renderSprite();
    //3Dスプライト
    mRenderer->renderSprite3D();

    if (isGameMode()) {
        //3Dスプライトを描画する
        mSpriteManager->draw3Ds(view, proj);
    }

    //2Dスプライト
    auto proj2D = Matrix4::identity;
    mRenderer->renderSprite2D(proj2D);

    if (isGameMode()) {
        //メッシュ描画済みテクスチャを描画する
        mMeshRenderOnTextureManager->drawTextures(proj2D);
        //2Dスプライト描画
        mSpriteManager->drawComponents(proj2D);
        //テキスト描画
        mTextDrawer->drawAll(proj2D);
    }

#ifdef _DEBUG
    mEngineManager->draw(mMode, *mRenderer, proj2D);
#endif // _DEBUG
}

void SceneManager::onChangeGameMode() {
    mMode = EngineMode::GAME;
    mMeshManager->registerThisToMeshRenderer();
    mEngineManager->debug().getDebugLayer().hierarchy().setGameObjectsGetter(mGameObjectManager.get());
}

void SceneManager::onChangeMapEditorMode() {
    mMode = EngineMode::MAP_EDITOR;
    mEngineManager->onChangeMapEditorMode();
    mEngineManager->debug().getDebugLayer().hierarchy().setGameObjectsGetter(mEngineManager->getMapEditorMeshManager().getGameObjects());
}

void SceneManager::onChangeModelViewerMode() {
    mMode = EngineMode::MODEL_VIEWER;
    mEngineManager->onChangeModelViewerMode();
}

EngineMode SceneManager::getMode() const {
    return mMode;
}

void SceneManager::change() {
    mGameObjectManager->clear(mRemoveExclusionTags);
    mMeshManager->clear();
    mSpriteManager->clear();
}

void SceneManager::createScene(const std::string& name) {
    //シーン作成
    auto scene = GameObjectCreater::create(name);
    //シーンコンポーネント取得
    mCurrentScene = scene->componentManager().getComponent<Scene>();
}

void SceneManager::choiceBeginScene() {
    if (mBeginScene == EngineModeName::MAP_EDITOR) {
        onChangeMapEditorMode();
    } else if (mBeginScene == EngineModeName::MODEL_VIEWER) {
        onChangeModelViewerMode();
    } else {
        onChangeGameMode();
    }
    createScene(mReleaseScene);
}

bool SceneManager::isGameMode() const {
    return (mMode == EngineMode::GAME);
}
