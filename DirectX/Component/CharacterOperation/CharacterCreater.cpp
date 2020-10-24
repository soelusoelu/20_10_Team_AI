#include "CharacterCreater.h"
#include "../Mesh/MeshComponent.h"
#include "../Sprite/SpriteComponent.h"
#include "../../Collision/Collision.h"
#include "../../GameObject/GameObjectFactory.h"
#include "../../Input/Input.h"
#include "../../Transform/Transform2D.h"
#include "../../System/Window.h"

CharacterCreater::CharacterCreater(GameObject& gameObject) :
    Component(gameObject),
    mSprite(nullptr),
    mClickedSprite(false)
{
}

CharacterCreater::~CharacterCreater() = default;

void CharacterCreater::start() {
    mSprite = getComponent<SpriteComponent>();
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
            createCharacter(1);

            //多重生成を阻止するため
            mClickedSprite = false;
        }
    }
    //マウスの左ボタンを離した瞬間だったら
    if (mouse.getMouseButtonUp(MouseCode::LeftButton)) {
        mClickedSprite = false;
    }
}

bool CharacterCreater::selectSprite(const Vector2& mousePos) {
    //計算に必要な要素を取得する
    const auto& st = mSprite->transform();
    auto compen = Window::getWindowCompensate();
    auto sPos = st.getPosition() * compen;
    auto sScale = st.getScale() * compen;
    auto texSize = mSprite->getTextureSize() * sScale;

    //スプライトをもとに矩形作成
    Square square(sPos, sPos + texSize);

    //矩形の中にマウスの座標が含まれているか
    return (square.contains(mousePos));
}

void CharacterCreater::createCharacter(int id) {
    //仮
    if (id == 0) {
        GameObjectCreater::create("SphereTest");
    } else if (id == 1) {
        GameObjectCreater::create("BoxTest");
    }
}
