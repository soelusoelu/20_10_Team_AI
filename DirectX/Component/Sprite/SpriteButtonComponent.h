#pragma once

#include "../Component.h"
#include "../../Math/Math.h"
#include <functional>
#include <memory>

class SpriteComponent;
class Subject;

class SpriteButtonComponent : public Component {
    using SpritePtr = std::shared_ptr<SpriteComponent>;

public:
    SpriteButtonComponent(GameObject& gameObject);
    ~SpriteButtonComponent();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;

    //スプライトを設定する
    void setSprite(const SpritePtr& sprite);
    //クリックした際のコールバック
    void callbackClick(const std::function<void()>& onClick);

private:
    SpriteButtonComponent(const SpriteButtonComponent&) = delete;
    SpriteButtonComponent& operator=(const SpriteButtonComponent&) = delete;

    //スプライトの設定を共通化する
    void spriteSettings(SpriteComponent& dst, const SpriteComponent& src);
    //アクティブにするスプライトの設定
    void activeSpriteSettings(const SpritePtr& active, const SpritePtr& nonActive);

private:
    SpritePtr mSprite;
    SpritePtr mSelectingSprite;
    std::unique_ptr<Subject> mCallbackClick;
    bool mPreviousContains;
};
