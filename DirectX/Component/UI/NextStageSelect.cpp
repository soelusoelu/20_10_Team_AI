#include "NextStageSelect.h"
#include "../Sprite/SpriteButtonComponent.h"
#include "../Sprite/SpriteComponent.h"
#include "../Text/Text.h"
#include "../../System/Game.h"
#include "../../Transform/Transform2D.h"

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
