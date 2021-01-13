#include "PointLight.h"
#include "../DirectX/InputElement.h"
#include "../Mesh/IMeshLoader.h"
#include "../Mesh/Material.h"
#include "../System/AssetsManager.h"
#include "../System/Shader/Shader.h"
#include "../Utility/LevelLoader.h"

PointLight::PointLight() :
    shader(nullptr),
    radius(0.f),
    mMeshFileName("") {
}

PointLight::~PointLight() = default;

void PointLight::loadProperties(const rapidjson::Value & inObj) {
    const auto& obj = inObj["pointLight"];
    if (obj.IsObject()) {
        JsonHelper::getString(obj, "pointLightMeshFileName", &mMeshFileName);
    }
}

void PointLight::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const {
    rapidjson::Value props(rapidjson::kObjectType);
    JsonHelper::setString(alloc, &props, "pointLightMeshFileName", mMeshFileName);

    inObj.AddMember("pointLight", props, alloc);
}

void PointLight::initialize() {
    //mesh = World::instance().assetsManager().createMeshLoader(mMeshFileName);
    //mesh->setInitMaterials(&materials);
    //radius = mesh->getRadius();
    //shader = World::instance().assetsManager().createShader("PointLight.hlsl");
}
