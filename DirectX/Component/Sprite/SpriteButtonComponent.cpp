#include "SpriteButtonComponent.h"
#include "../Sprite/SpriteComponent.h"
#include "../../Device/Subject.h"
#include "../../Input/Input.h"
#include "../../Sprite/SpriteUtility.h"
#include "../../Transform/Transform2D.h"
#include "../../Utility/LevelLoader.h"
#include <string>

SpriteButtonComponent::SpriteButtonComponent(GameObject& gameObject)
    : Component(gameObject)
    , mSprite(nullptr)
    , mSelectingSprite(nullptr)
    , mCallbackClick(std::make_unique<Subject>())
    , mPreviousContains(false)
{
}

SpriteButtonComponent::~SpriteButtonComponent() = default;

void SpriteButtonComponent::start() {
    mSprite = getComponent<SpriteComponent>();
    mSelectingSprite->setActive(false);
    if (mSprite) {
        spriteSettings(*mSelectingSprite, *mSprite);
    }
}

void SpriteButtonComponent::update() {
    const auto& mouse = Input::mouse();

    auto contains = SpriteUtility::contains(*mSprite, mouse.getMousePosition());
    if (contains && !mPreviousContains) {
        activeSpriteSettings(mSelectingSprite, mSprite);
    } else if (!contains && mPreviousContains) {
        activeSpriteSettings(mSprite, mSelectingSprite);
    }

    //マウスの左ボタンを押していれば通知を送る
    if (contains && mouse.getMouseButtonDown(MouseCode::LeftButton)) {
        mCallbackClick->notify();
    }

    mPreviousContains = contains;
}
void SpriteButtonComponent::loadProperties(const rapidjson::Value& inObj) {
    if (std::string fileName;  JsonHelper::getString(inObj, "selectingSpriteFileName", &fileName)) {
        mSelectingSprite = addComponent<SpriteComponent>("SpriteComponent");
        mSelectingSprite->setTextureFromFileName(fileName);
    }
}

void SpriteButtonComponent::setSprite(const SpritePtr& sprite) {
    mSprite = sprite;
}

void SpriteButtonComponent::callbackClick(const std::function<void()>& onClick) {
    mCallbackClick->addObserver(onClick);
}

void SpriteButtonComponent::spriteSettings(SpriteComponent& dst, const SpriteComponent& src) {
    auto& dstT = dst.transform();
    const auto& srcT = src.transform();

    dstT.setPosition(srcT.getPosition());
    dstT.setScale(srcT.getScale());
    dstT.setPivot(srcT.getPivotEnum());
    dst.setColor(src.getColor());
    dst.setAlpha(src.getAlpha());
}

void SpriteButtonComponent::activeSpriteSettings(const SpritePtr& active, const SpritePtr& nonActive) {
    if (active) {
        active->setActive(true);
    }
    if (nonActive) {
        nonActive->setActive(false);
    }
}
