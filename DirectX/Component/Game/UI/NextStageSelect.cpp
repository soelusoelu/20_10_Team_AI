#include "NextStageSelect.h"
#include "../../Engine/Sprite/SpriteButtonComponent.h"
#include "../../Engine/Sprite/SpriteComponent.h"
#include "../../Engine/Text/Text.h"
#include "../../../System/Game.h"
#include "../../../Transform/Transform2D.h"

NextStageSelect::NextStageSelect(GameObject& gameObject)
    : Component(gameObject)
{
}

NextStageSelect::~NextStageSelect() = default;

void NextStageSelect::start() {
    const auto& s = getComponent<SpriteComponent>();
    const auto& text = getComponent<Text>();
    text->setPosition(s->transform().getPosition());
}

void NextStageSelect::callbackButtonClick(const std::function<void()>& callback) {
    getComponent<SpriteButtonComponent>()->callbackClick(callback);
}
