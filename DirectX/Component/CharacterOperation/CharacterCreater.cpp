#include "CharacterCreater.h"
#include "CharacterCost.h"
#include "../Mesh/MeshComponent.h"
#include "../Sprite/SpriteComponent.h"
#include "../Text/Text.h"
#include "../../Collision/Collision.h"
#include "../../DebugLayer/Debug.h"
#include "../../GameObject/GameObjectFactory.h"
#include "../../Input/Input.h"
#include "../../Transform/Transform2D.h"
#include "../../Sprite/SpriteUtility.h"
#include "../../System/AssetsManager.h"
#include "../../System/Window.h"
#include "../../Utility/LevelLoader.h"
#include "../../Utility/StringUtil.h"

CharacterCreater::CharacterCreater(GameObject& gameObject)
    : Component(gameObject)
    , mCost(nullptr)
    , mClickingSprite(false)
    , mClickedSpriteID(0)
    , mSpriteStartPos(Vector2::zero)
    , mSpriteScale(Vector2::one)
    , mSpriteSpace(0.f)
    , mSpritePivot(Pivot::LEFT_TOP)
    , mNonActiveAlpha(0.f)
{
}

CharacterCreater::~CharacterCreater() = default;

void CharacterCreater::start() {
    mCost = getComponent<CharacterCost>();
    mCost->callbackUpdateCost([&] { onUpdateCost(); });
}

void CharacterCreater::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getVector2(inObj, "spriteStartPosition", &mSpriteStartPos);
    JsonHelper::getVector2(inObj, "spriteScale", &mSpriteScale);
    JsonHelper::getFloat(inObj, "spriteSpace", &mSpriteSpace);
    std::string pivot;
    if (JsonHelper::getString(inObj, "spritePivot", &pivot)) {
        PivotFunc::stringToPivot(pivot, &mSpritePivot);
    }
    JsonHelper::getFloat(inObj, "nonActiveAlpha", &mNonActiveAlpha);
}

void CharacterCreater::create(std::shared_ptr<GameObject>& out, int& cost) {
    //マウスインターフェイスを取得
    const auto& mouse = Input::mouse();

    //マウスの左ボタンを押した瞬間だったら
    if (mouse.getMouseButtonDown(MouseCode::LeftButton)) {
        mClickingSprite = selectSprite(mouse.getMousePosition());
    }
    //マウスの左ボタンを押し続けていたら
    if (mouse.getMouseButton(MouseCode::LeftButton)) {
        clickingLeftMouseButton(out, cost, mouse.getMousePosition());
    }
    //マウスの左ボタンを離した瞬間だったら
    if (mouse.getMouseButtonUp(MouseCode::LeftButton)) {
        mClickingSprite = false;
    }
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
        JsonHelper::getString(charaObj, "sprite", &chara.spriteFileName);
        JsonHelper::getInt(charaObj, "cost", &chara.cost);
        chara.sprite = addComponent<SpriteComponent>("SpriteComponent");
        chara.sprite->setTextureFromFileName(chara.spriteFileName);
        chara.isActive = true;

        //キャラコスト表示用にテキストコンポーネントを追加する
        mTexts.emplace_back(addComponent<Text>("Text"));
    }

    //初期化
    initialize();
}

bool CharacterCreater::isOperating() const {
    return mClickingSprite;
}

void CharacterCreater::initialize() {
    //スプライトの位置を調整する
    for (int i = 0; i < mCharactersInfo.size(); ++i) {
        auto& s = mCharactersInfo[i].sprite;
        auto& st = s->transform();
        auto texSize = s->getTextureSize() * mSpriteScale;
        st.setScale(mSpriteScale);
        st.setPivot(mSpritePivot);

        //スプライトの位置を計算し配置していく
        st.setPosition(mSpriteStartPos + Vector2(texSize.x * i + mSpriteSpace * i, 0.f));

        //キャラコスト表示位置を設定する
        auto& text = mTexts[i];
        text->setText(StringUtil::intToString(mCharactersInfo[i].cost));
        text->setPosition(st.getPosition() + Vector2::up * texSize.y);
        text->setScale(Vector2::one * 0.75f);
    }
}

void CharacterCreater::clickingLeftMouseButton(std::shared_ptr<GameObject>& out, int& cost, const Vector2& mousePos) {
    //スプライトをクリックしていないなら終了
    if (!mClickingSprite) {
        return;
    }

    //マウス位置がクリックしたスプライトの内にあるなら終了
    if (SpriteUtility::contains(*mCharactersInfo[mClickedSpriteID].sprite, mousePos)) {
        return;
    }

    //IDに対応するキャラクターを生成する
    out = createCharacter(mClickedSpriteID);
    //IDに対応するコストを取得する
    cost = mCharactersInfo[mClickedSpriteID].cost;

    //多重生成を阻止するため
    mClickingSprite = false;
}

bool CharacterCreater::selectSprite(const Vector2& mousePos) {
    //すべてのスプライトで検証する
    for (int i = 0; i < mCharactersInfo.size(); ++i) {
        auto& chara = mCharactersInfo[i];
        //非アクティブならスキップ
        if (!chara.isActive) {
            continue;
        }

        //スプライトの中にマウスの座標が含まれているか
        if (SpriteUtility::contains(*chara.sprite, mousePos)) {
            mClickedSpriteID = i;
            return true;
        }
    }

    //すべてのスプライトの矩形範囲外
    return false;
}

std::shared_ptr<GameObject> CharacterCreater::createCharacter(int id) {
    //IDに対応するメッシュを作成
    const auto& chara = mCharactersInfo[id];
    //キャラ分のコストを減らす
    mCost->useCost(chara.cost);
    //キャラを生成し、返す
    return GameObjectCreater::create(chara.fileName);
}

void CharacterCreater::onUpdateCost() {
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

    for (size_t i = 0; i < mCharactersInfo.size(); ++i) {
        //キャラのコストが現在のコストより多ければ使用不可にする
        auto& chara = mCharactersInfo[i];
        if (chara.cost > mCost->getCost()) {
            chara.sprite->setAlpha(mNonActiveAlpha);
            chara.isActive = false;
            mTexts[i]->setAlpha(mNonActiveAlpha);
        } else {
            chara.sprite->setAlpha(1.f);
            chara.isActive = true;
            mTexts[i]->setAlpha(1.f);
        }
    }
}
