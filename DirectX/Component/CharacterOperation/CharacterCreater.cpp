#include "CharacterCreater.h"
#include "CharacterCost.h"
#include "../Mesh/MeshComponent.h"
#include "../Sprite/SpriteComponent.h"
#include "../../Collision/Collision.h"
#include "../../DebugLayer/Debug.h"
#include "../../GameObject/GameObjectFactory.h"
#include "../../Input/Input.h"
#include "../../Transform/Transform2D.h"
#include "../../System/AssetsManager.h"
#include "../../System/Window.h"
#include "../../Utility/LevelLoader.h"

CharacterCreater::CharacterCreater(GameObject& gameObject)
    : Component(gameObject)
    , mCost(nullptr)
    , mClickedSprite(false)
    , mClickedSpriteID(0)
    , mSpriteStartPos(Vector2::zero)
    , mSpriteScale(Vector2::zero)
    , mSpriteSpace(0.f) {
}

CharacterCreater::~CharacterCreater() = default;

void CharacterCreater::start() {
    mCost = getComponent<CharacterCost>();

    //スプライトの位置を調整する
    for (int i = 0; i < mCharactersInfo.size(); ++i) {
        auto& s = mCharactersInfo[i].sprite;
        auto& st = s->transform();
        auto texSize = s->getTextureSize() * mSpriteScale;
        st.setScale(mSpriteScale);

        //スプライトの位置を計算し配置していく
        st.setPosition(mSpriteStartPos + Vector2(texSize.x * i + mSpriteSpace * i, 0.f));
    }
}

void CharacterCreater::update() {
    //マウスインターフェイスを取得
    const auto& mouse = Input::mouse();

    //マウスの左ボタンを押した瞬間だったら
    if (mouse.getMouseButtonDown(MouseCode::LeftButton)) {
        mClickedSprite = selectSprite(mouse.getMousePosition());
    }
    //マウスの左ボタンを押し続けていたら
    if (mouse.getMouseButton(MouseCode::LeftButton)) {
        //スプライトをクリックしていないなら終了
        if (!mClickedSprite) {
            return;
        }

        //マウス位置がスプライトの外にあるなら対応するキャラクターを生成する
        if (!selectSprite(mouse.getMousePosition())) {
            //対応するIDのキャラクターを生成
            createCharacter(mClickedSpriteID);

            //コストオーバーしたスプライトの操作
            spriteCostOver();

            //多重生成を阻止するため
            mClickedSprite = false;
        }
    }
    //マウスの左ボタンを離した瞬間だったら
    if (mouse.getMouseButtonUp(MouseCode::LeftButton)) {
        mClickedSprite = false;
    }
}

void CharacterCreater::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getVector2(inObj, "spriteStartPosition", &mSpriteStartPos);
    JsonHelper::getVector2(inObj, "spriteScale", &mSpriteScale);
    JsonHelper::getFloat(inObj, "spriteSpace", &mSpriteSpace);
}

void CharacterCreater::loadCharacter(const rapidjson::Value& inObj) {
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
        JsonHelper::getString(charaObj, "sprite", &chara.spriteFileName);
        JsonHelper::getInt(charaObj, "cost", &chara.cost);
        chara.sprite = addComponent<SpriteComponent>("SpriteComponent");
        chara.sprite->setTextureFromFileName(chara.spriteFileName);
        chara.isActive = true;
    }
}

bool CharacterCreater::selectSprite(const Vector2& mousePos) {
    //ウィンドウ補正値を取得する
    auto compen = Window::getWindowCompensate();
    //すべてのスプライトで検証する
    for (int i = 0; i < mCharactersInfo.size(); ++i) {
        auto& chara = mCharactersInfo[i];
        //非アクティブならスキップ
        if (!chara.isActive) {
            continue;
        }

        //計算に必要な要素を取得する
        auto& s = chara.sprite;
        const auto& st = s->transform();
        auto sPos = st.getPosition() * compen;
        auto sScale = st.getScale() * compen;
        auto texSize = s->getTextureSize() * sScale;

        //スプライトをもとに矩形作成
        Square square(sPos, sPos + texSize);

        //矩形の中にマウスの座標が含まれているか
        if (square.contains(mousePos)) {
            mClickedSpriteID = i;
            return true;
        }
    }

    //すべてのスプライトの矩形範囲外
    return false;
}

void CharacterCreater::createCharacter(int id) {
    //IDに対応するメッシュを作成
    const auto& chara = mCharactersInfo[id];
    GameObjectCreater::create(chara.fileName);
    //キャラ分のコストを減らす
    mCost->useCost(chara.cost);
}

void CharacterCreater::spriteCostOver() {
    for (auto&& chara : mCharactersInfo) {
        //キャラのコストが現在のコストより多ければ使用不可にする
        if (chara.cost > mCost->getCost()) {
            chara.sprite->setAlpha(0.2f);
            chara.isActive = false;
        } else {
            chara.sprite->setAlpha(1.f);
            chara.isActive = true;
        }
    }
}
