#include "Button.h"
#include "../Collision/Collision.h"
#include "../Input/Input.h"
#include "../Sprite/Sprite.h"
#include "../Transform/Transform2D.h"

Button::Button(const std::function<void()>& onClick, const Vector2& pos, const Vector2& dims) :
    mOnClick(onClick),
    mPosition(pos),
    mDimensions(dims),
    mHighlighted(false) {
}

Button::~Button() = default;

void Button::setClickFunc(const std::function<void()>& onClick) {
    mOnClick = onClick;
}

const Vector2& Button::getPosition() const {
    return mPosition;
}

void Button::setHighlighted(bool set) {
    mHighlighted = set;
}

bool Button::getHighlighted() const {
    return mHighlighted;
}

bool Button::containsPoint(const Vector2& pt) const {
    bool no = pt.x < (mPosition.x) ||
        pt.x >(mPosition.x + mDimensions.x) ||
        pt.y < (mPosition.y) ||
        pt.y >(mPosition.y + mDimensions.y);
    return !no;
}

bool Button::clickButton(const Vector2& pt) const {
    if (!Input::mouse().getMouseButtonDown(MouseCode::LeftButton)) {
        return false;
    }

    if (!containsPoint(pt)) {
        return false;
    }

    onClick();
    return true;
}

void Button::onClick() const {
    if (mOnClick) {
        mOnClick();
    }
}



SpriteButton::SpriteButton(const std::function<void()>& onClick, const std::string& fileName, const Vector2& pos) :
    mOnClick(onClick),
    mSprite(std::make_unique<Sprite>(fileName)),
    mHighlighted(false) {
    mSprite->transform().setPosition(pos);
}

SpriteButton::~SpriteButton() = default;

void SpriteButton::setClickFunc(const std::function<void()>& onClick) {
    mOnClick = onClick;
}

const Vector2& SpriteButton::getPosition() const {
    return mSprite->transform().getPosition();
}

void SpriteButton::setHighlighted(bool set) {
    mHighlighted = set;
}

bool SpriteButton::getHighlighted() const {
    return mHighlighted;
}

bool SpriteButton::containsPoint(const Vector2& pt) const {
    //計算に必要な要素を取得する
    const auto& t = mSprite->transform();
    const auto& scale = t.getScale();
    const auto& pivot = t.getPivot() * scale;
    const auto& compenPos = t.getPosition() - pivot;
    const auto& texSize = mSprite->getTextureSize() * scale;

    //スプライトをもとに矩形作成
    Square square(compenPos, compenPos + texSize);

    //矩形の中にマウスの座標が含まれているか
    return square.contains(pt);
}

bool SpriteButton::clickButton(const Vector2& pt) const {
    if (!Input::mouse().getMouseButtonDown(MouseCode::LeftButton)) {
        return false;
    }

    if (!containsPoint(pt)) {
        return false;
    }

    onClick();
    return true;
}

void SpriteButton::onClick() const {
    if (mOnClick) {
        mOnClick();
    }
}

void SpriteButton::draw(const Matrix4& proj) const {
    mSprite->transform().computeWorldTransform();
    mSprite->draw(proj);
}
