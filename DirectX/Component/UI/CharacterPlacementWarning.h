#pragma once

#include "../Component.h"
#include <memory>

class SpriteComponent;

class CharacterPlacementWarning : public Component {
public:
    CharacterPlacementWarning(GameObject& gameObject);
    ~CharacterPlacementWarning();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    void enabled();

private:
    CharacterPlacementWarning(const CharacterPlacementWarning&) = delete;
    CharacterPlacementWarning& operator=(const CharacterPlacementWarning&) = delete;

private:
    std::shared_ptr<SpriteComponent> mSprite;
    float mEasingSpeed;
    float mEaseValue;
};
