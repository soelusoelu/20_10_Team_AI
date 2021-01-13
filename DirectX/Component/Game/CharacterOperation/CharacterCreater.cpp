#include "CharacterCreater.h"
#include "CharacterCost.h"
#include "CharacterCreateSpriteOperation.h"
#include "../../Engine/Mesh/MeshComponent.h"
#include "../../Engine/Sprite/SpriteComponent.h"
#include "../../../GameObject/GameObjectFactory.h"
#include "../../../System/AssetsManager.h"
#include "../../../Utility/LevelLoader.h"
#include <cassert>

CharacterCreater::CharacterCreater(GameObject& gameObject)
    : Component(gameObject)
    , mSpriteOperator(nullptr)
    , mCost(nullptr)
{
}

CharacterCreater::~CharacterCreater() = default;

void CharacterCreater::start() {
    mSpriteOperator = getComponent<CharacterCreateSpriteOperation>();
    mSpriteOperator->setCharacterCreater(this);
    mCost = getComponent<CharacterCost>();
}

const CharacterCreateInfo& CharacterCreater::getCharacterCreateInfo(unsigned index) const {
    assert(index < mCharactersInfo.size());
    return mCharactersInfo[index];
}

unsigned CharacterCreater::getCharacterCount() const {
    return mCharactersInfo.size();
}

void CharacterCreater::originalUpdate() {
    mSpriteOperator->originalUpdate();
}

void CharacterCreater::create(std::shared_ptr<GameObject>& out, int& cost) {
    //IDに対応するキャラクターを作成
    const auto& chara = mCharactersInfo[mSpriteOperator->getCreateCharacterID()];
    //キャラ分のコストを減らす
    mCost->useCost(chara.cost);
    //引数にコストを入れる
    cost = chara.cost;
    //キャラを生成する
    out = GameObjectCreater::create(chara.fileName);
}

void CharacterCreater::receiveExternalData(const rapidjson::Value& inObj) {
    rapidjson::Document doc;
    if (!LevelLoader::loadJSON(doc, "Characters.json")) {
        return;
    }
    if (!doc.IsObject()) {
        return;
    }

    std::vector<std::string> characters;
    JsonHelper::getStringArray(inObj, "characters", &characters);
    //配列の要素数分拡張
    mCharactersInfo.resize(characters.size());
    for (size_t i = 0; i < characters.size(); ++i) {
        const auto& name = characters[i];
        const auto& charaObj = doc[name.c_str()];
        if (!charaObj.IsObject()) {
            continue;
        }

        auto& chara = mCharactersInfo[i];
        chara.fileName = name;
        std::string mesh;
        if (JsonHelper::getString(charaObj, "mesh", &mesh)) {
            //メッシュを事前に読み込んでおく
            AssetsManager::instance().loadMesh(mesh);
        }
        if (JsonHelper::getString(charaObj, "sprite", &chara.spriteFileName)) {
            //スプライトを追加する
            mSpriteOperator->addSprite(chara.spriteFileName);
        }
        JsonHelper::getInt(charaObj, "cost", &chara.cost);
    }

    //初期化
    mSpriteOperator->initialize();
}

bool CharacterCreater::isOperating() const {
    return mSpriteOperator->isOperating();
}
