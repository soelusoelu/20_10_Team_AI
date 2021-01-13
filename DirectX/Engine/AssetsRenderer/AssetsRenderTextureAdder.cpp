#include "AssetsRenderTextureAdder.h"
#include "../../Device/Button.h"
#include "../../Input/Input.h"
#include "../../Math/Math.h"
#include "../../Utility/FileUtil.h"
#include "../../Utility/LevelLoader.h"

AssetsRenderTextureAdder::AssetsRenderTextureAdder()
    : mButton(nullptr)
    , mAssetsAdder(nullptr)
    , mRenderPosition()
{
}

AssetsRenderTextureAdder::~AssetsRenderTextureAdder() = default;

void AssetsRenderTextureAdder::loadProperties(const rapidjson::Value& inObj) {
    const auto& artaObj = inObj["assetsRenderTextureAdder"];
    if (artaObj.IsObject()) {
        JsonHelper::getVector2(artaObj, "renderPosition", &mRenderPosition);
        JsonHelper::getString(artaObj, "spriteButtonFilePath", &mSpriteFilePath);
    }
}

void AssetsRenderTextureAdder::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const {
    rapidjson::Value props(rapidjson::kObjectType);
    JsonHelper::setVector2(alloc, &props, "renderPosition", mRenderPosition);
    JsonHelper::setString(alloc, &props, "spriteButtonFilePath", mSpriteFilePath);

    inObj.AddMember("assetsRenderTextureAdder", props, alloc);
}

void AssetsRenderTextureAdder::initialize(IAddAssets* adder) {
    mAssetsAdder = adder;
    mButton = std::make_unique<SpriteButton>([&] { onClickButton(); }, mSpriteFilePath, mRenderPosition);
}

void AssetsRenderTextureAdder::update() {
    mButton->clickButton(Input::mouse().getMousePosition());
}

void AssetsRenderTextureAdder::draw(const Matrix4& proj) {
    mButton->draw(proj);
}

void AssetsRenderTextureAdder::onClickButton() {
    if (std::string outFilePath, outFileName; FileUtil::openFileDialog(outFilePath, outFileName)) {
        //絶対パスからアセットディレクトリ部分を抜き出す
        const auto& assetsDir = FileUtil::getAssetsFromAbsolutePath(outFilePath);
        mAssetsAdder->add(assetsDir);
    }
}
