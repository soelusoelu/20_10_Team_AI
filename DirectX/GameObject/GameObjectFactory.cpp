#include "GameObjectFactory.h"
#include "GameObject.h"
#include "../Component/Component.h"
#include "../Component/ComponentManager.h"
#include "../Component/Engine/Camera/Camera.h"
#include "../Component/Engine/CollideOperation/AABBMouseScaler.h"
#include "../Component/Engine/CollideOperation/AABBSelector.h"
#include "../Component/Engine/CollideOperation/CollideAdder.h"
#include "../Component/Engine/CollideOperation/CollideMouseOperator.h"
#include "../Component/Engine/CollideOperation/MeshAdder.h"
#include "../Component/Engine/Collider/AABBCollider.h"
#include "../Component/Engine/Collider/CircleCollider.h"
#include "../Component/Engine/Collider/SphereCollider.h"
#include "../Component/Engine/Light/DirectionalLight.h"
#include "../Component/Engine/Light/PointLightComponent.h"
#include "../Component/Engine/Mesh/MeshComponent.h"
#include "../Component/Engine/Mesh/MeshMaterial.h"
#include "../Component/Engine/Mesh/MeshOutLine.h"
#include "../Component/Engine/Mesh/MeshRenderer.h"
#include "../Component/Engine/Mesh/MeshShader.h"
#include "../Component/Engine/Mesh/SkinMeshComponent.h"
#include "../Component/Engine/Other/GameObjectSaveAndLoader.h"
#include "../Component/Engine/Other/SaveThis.h"
#include "../Component/Engine/Sample/RayMouse.h"
#include "../Component/Engine/Sample/WaveformRenderSample.h"
#include "../Component/Engine/Scene/MapEditor.h"
#include "../Component/Engine/Sound/ListenerComponent.h"
#include "../Component/Engine/Sound/SoundComponent.h"
#include "../Component/Engine/Sprite/Sprite3D.h"
#include "../Component/Engine/Sprite/SpriteButtonComponent.h"
#include "../Component/Engine/Sprite/SpriteComponent.h"
#include "../Component/Engine/Text/Text.h"
#include "../Component/Engine/Text/TextFloat.h"
#include "../Component/Engine/Text/TextNumber.h"
#include "../Component/Game/Camera/CameraMove.h"
#include "../Component/Game/AI/ASAI.h"
#include "../Component/Game/AI/ASCell.h"
#include "../Component/Game/AI/ASCellManager.h"
#include "../Component/Game/Character/CharacterAction.h"
#include "../Component/Game/Character/CharacterCommonComponents.h"
#include "../Component/Game/Character/CharacterDead.h"
#include "../Component/Game/Character/CharacterManager.h"
#include "../Component/Game/Character/HitPointRenderer.h"
#include "../Component/Game/Character/OverlapPrevention.h"
#include "../Component/Game/Character/PhaseChangeSaver.h"
#include "../Component/Game/Character/SimpleCharacter.h"
#include "../Component/Game/CharacterOperation/CharacterCost.h"
#include "../Component/Game/CharacterOperation/CharacterCreater.h"
#include "../Component/Game/CharacterOperation/CharacterCreateSpriteOperation.h"
#include "../Component/Game/CharacterOperation/CharacterDeleter.h"
#include "../Component/Game/CharacterOperation/CharacterOperation.h"
#include "../Component/Game/CharacterOperation/CharacterSelector.h"
#include "../Component/Game/CharacterOperation/CostRenderer.h"
#include "../Component/Game/CharacterOperation/DragAndDropCharacter.h"
#include "../Component/Game/EnemyOperation/EnemyCreater.h"
#include "../Component/Game/EnemyOperation/EnemyOperation.h"
#include "../Component/Game/GameState/GameClear.h"
#include "../Component/Game/GameState/GameJudge.h"
#include "../Component/Game/GameState/GameReset.h"
#include "../Component/Game/GameState/GameStart.h"
#include "../Component/Game/GameState/StageClear.h"
#include "../Component/Game/GameState/StageFail.h"
#include "../Component/Game/GameState/StageFailArrow.h"
#include "../Component/Game/Map/Map.h"
#include "../Component/Game/Other/HitPointComponent.h"
#include "../Component/Game/Scene/GamePlay.h"
#include "../Component/Game/Scene/StageSelect.h"
#include "../Component/Game/Scene/Title.h"
#include "../Component/Game/UI/CharacterPlacementWarning.h"
#include "../Component/Game/UI/ClearToStageSelect.h"
#include "../Component/Game/UI/GameEnd.h"
#include "../Component/Game/UI/NextStageSelect.h"
#include "../Component/Game/UI/Menu/Menu.h"
#include "../Component/Game/UI/Menu/MenuClose.h"
#include "../Component/Game/UI/Menu/MenuGameEnd.h"
#include "../Component/Game/UI/Menu/MenuToStageSelect.h"
#include "../Component/Game/UI/Menu/MenuToTitle.h"
#include "../Component/Game/UI/Menu/MenuWindow.h"
#include "../Component/Game/UI/Menu/MenuWindowInitializer.h"
#include "../Engine/DebugManager/DebugUtility/Debug.h"
#include "../System/GlobalFunction.h"
#include "../Utility/LevelLoader.h"
#include <cassert>

//練習がてら、ちょい楽できるように
#define ADD_COMPONENT(className) { mComponents.emplace((#className), &Component::create<className>); }

GameObjectFactory::GameObjectFactory() {
    assert(!mInstantiated);
    mInstantiated = true;

#pragma region Engine
    ADD_COMPONENT(Camera);

    ADD_COMPONENT(AABBMouseScaler);
    ADD_COMPONENT(AABBSelector);
    ADD_COMPONENT(CollideAdder);
    ADD_COMPONENT(CollideMouseOperator);
    ADD_COMPONENT(MeshAdder);

    ADD_COMPONENT(AABBCollider);
    ADD_COMPONENT(CircleCollider);
    ADD_COMPONENT(SphereCollider);

    ADD_COMPONENT(DirectionalLight);
    ADD_COMPONENT(PointLightComponent);

    ADD_COMPONENT(MeshComponent);
    ADD_COMPONENT(MeshMaterial);
    ADD_COMPONENT(MeshOutLine);
    ADD_COMPONENT(MeshRenderer);
    ADD_COMPONENT(MeshShader);
    ADD_COMPONENT(SkinMeshComponent);

    ADD_COMPONENT(GameObjectSaveAndLoader);
    ADD_COMPONENT(SaveThis);

    ADD_COMPONENT(RayMouse);
    ADD_COMPONENT(WaveformRenderSample);

    ADD_COMPONENT(MapEditor);

    ADD_COMPONENT(ListenerComponent);
    ADD_COMPONENT(SoundComponent);

    ADD_COMPONENT(Sprite3D);
    ADD_COMPONENT(SpriteButtonComponent);
    ADD_COMPONENT(SpriteComponent);

    ADD_COMPONENT(Text);
    ADD_COMPONENT(TextFloat);
    ADD_COMPONENT(TextNumber);
#pragma endregion

#pragma region Game
    ADD_COMPONENT(ASAI);

    ADD_COMPONENT(CameraMove);

    ADD_COMPONENT(CharacterAction);
    ADD_COMPONENT(CharacterCommonComponents);
    ADD_COMPONENT(CharacterDead);
    ADD_COMPONENT(CharacterManager);
    ADD_COMPONENT(HitPointRenderer);
    ADD_COMPONENT(OverlapPrevention);
    ADD_COMPONENT(PhaseChangeSaver);
    ADD_COMPONENT(SimpleCharacter);

    ADD_COMPONENT(CharacterCost);
    ADD_COMPONENT(CharacterCreater);
    ADD_COMPONENT(CharacterCreateSpriteOperation);
    ADD_COMPONENT(CharacterDeleter);
    ADD_COMPONENT(CharacterOperation);
    ADD_COMPONENT(CharacterSelector);
    ADD_COMPONENT(CostRenderer);
    ADD_COMPONENT(DragAndDropCharacter);

    ADD_COMPONENT(EnemyCreater);
    ADD_COMPONENT(EnemyOperation);

    ADD_COMPONENT(GameClear);
    ADD_COMPONENT(GameJudge);
    ADD_COMPONENT(GameReset);
    ADD_COMPONENT(GameStart);
    ADD_COMPONENT(StageClear);
    ADD_COMPONENT(StageFail);
    ADD_COMPONENT(StageFailArrow);

    ADD_COMPONENT(Map);

    ADD_COMPONENT(HitPointComponent);

    ADD_COMPONENT(GamePlay);
    ADD_COMPONENT(StageSelect);
    ADD_COMPONENT(Title);

    ADD_COMPONENT(CharacterPlacementWarning);
    ADD_COMPONENT(ClearToStageSelect);
    ADD_COMPONENT(GameEnd);
    ADD_COMPONENT(NextStageSelect);
    ADD_COMPONENT(Menu);
    ADD_COMPONENT(MenuClose);
    ADD_COMPONENT(MenuGameEnd);
    ADD_COMPONENT(MenuToStageSelect);
    ADD_COMPONENT(MenuToTitle);
    ADD_COMPONENT(MenuWindow);
    ADD_COMPONENT(MenuWindowInitializer);
#pragma endregion
}

GameObjectFactory::~GameObjectFactory() {
    mInstantiated = false;
}

std::shared_ptr<GameObject> GameObjectFactory::createGameObjectFromFile(const std::string& type, const std::string& directoryPath) {
    rapidjson::Document document;
    const auto& fileName = type + ".json";
    if (!LevelLoader::loadJSON(document, fileName, directoryPath)) {
        Debug::windowMessage(directoryPath + fileName + ": ファイルのロードに失敗しました");
        return nullptr;
    }

    return createGameObject(document, type, directoryPath);
}

std::shared_ptr<GameObject> GameObjectFactory::createGameObject(const rapidjson::Document& inDocument, const std::string& type, const std::string& directoryPath) {
    //タグを読み込む
    const auto& tag = loadTag(inDocument);
    //ゲームオブジェクトを生成
    auto gameObject = GameObject::create(type, tag);
    //プロパティを読み込む
    loadGameObjectProperties(*gameObject, inDocument);

    //継承コンポーネントがあれば取得
    loadPrototypeComponents(*gameObject, inDocument, directoryPath);
    //コンポーネントがあれば取得
    loadComponents(*gameObject, inDocument);

    //全コンポーネントのstartを呼び出す
    gameObject->componentManager().start();

    return gameObject;
}

std::string GameObjectFactory::loadTag(const rapidjson::Document& inDocument) {
    //初期タグをNoneにする
    std::string tag = "None";
    //タグ属性があれば読み込む
    JsonHelper::getString(inDocument, "tag", &tag);

    return tag;
}

void GameObjectFactory::loadGameObjectProperties(GameObject& gameObject, const rapidjson::Document& inDocument) {
    if (inDocument.HasMember("transform")) {
        gameObject.loadProperties(inDocument["transform"]);
    }
}

void GameObjectFactory::loadPrototypeComponents(GameObject& gameObject, const rapidjson::Document& inDocument, const std::string& directoryPath) const {
    //ファイルにprototypeメンバがなければ終了
    if (!inDocument.HasMember("prototype")) {
        return;
    }

    //継承コンポーネントのファイル名を取得する
    std::string prototype;
    JsonHelper::getString(inDocument, "prototype", &prototype);

    rapidjson::Document document;
    const auto& fileName = prototype + ".json";
    if (!LevelLoader::loadJSON(document, fileName, directoryPath)) {
        Debug::windowMessage(directoryPath + fileName + ": ファイルのロードに失敗しました");
        return;
    }

    //継承コンポーネント読み込み
    loadComponents(gameObject, document);
}

void GameObjectFactory::loadComponents(GameObject& gameObject, const rapidjson::Document& inDocument) const {
    //ファイルにcomponentsメンバがなければ終了
    if (!inDocument.HasMember("components")) {
        return;
    }

    const auto& components = inDocument["components"];
    //componentsメンバが配列じゃなければなければ終了
    if (!components.IsArray()) {
        return;
    }

    for (rapidjson::SizeType i = 0; i < components.Size(); ++i) {
        //各コンポーネントを読み込んでいく
        loadComponent(gameObject, components[i]);
    }
}

void GameObjectFactory::loadComponent(GameObject& gameObject, const rapidjson::Value& component) const {
    //有効なオブジェクトか
    if (!component.IsObject()) {
        return;
    }
    //有効な型名か
    std::string type;
    if (!isValidType(type, component)) {
        return;
    }
    //mapに存在するか
    auto itr = mComponents.find(type);
    if (itr == mComponents.end()) {
        Debug::windowMessage(type + "は有効な型ではありません");
        return;
    }
    //プロパティがあるか
    if (!component.HasMember("properties")) {
        return;
    }

    //新規コンポーネントを生成
    itr->second(gameObject, type, component["properties"]);
}

bool GameObjectFactory::isValidType(std::string& outType, const rapidjson::Value& inObj) const {
    return (JsonHelper::getString(inObj, "type", &outType));
}



void GameObjectCreater::initialize() {
    mFactory = new GameObjectFactory();
}

void GameObjectCreater::finalize() {
    safeDelete(mFactory);
}

std::shared_ptr<GameObject> GameObjectCreater::create(const std::string& type) {
    return mFactory->createGameObjectFromFile(type);
}
