#include "MenuWindowInitializer.h"
#include "MenuWindow.h"
#include "MenuClose.h"
#include "MenuToStageSelect.h"
#include "../../../GameObject/GameObject.h"
#include "../../../GameObject/GameObjectFactory.h"

MenuWindowInitializer::MenuWindowInitializer(GameObject& gameObject)
    : Component(gameObject)
{
}

MenuWindowInitializer::~MenuWindowInitializer() = default;

void MenuWindowInitializer::start() {
    const auto& menuWindow = getComponent<MenuWindow>();

    const auto& toStageSelect = GameObjectCreater::create("MenuToStageSelect")->componentManager().getComponent<MenuToStageSelect>();
    const auto& menuClose = GameObjectCreater::create("MenuClose")->componentManager().getComponent<MenuClose>();
    menuClose->setMenuWindow(menuWindow);

    menuWindow->addMenuButton(toStageSelect.get());
    menuWindow->addMenuButton(menuClose.get());
}
