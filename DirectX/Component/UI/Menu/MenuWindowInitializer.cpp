#include "MenuWindowInitializer.h"
#include "MenuWindow.h"
#include "MenuClose.h"
#include "MenuGameEnd.h"
#include "MenuToStageSelect.h"
#include "MenuToTitle.h"
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
    const auto& toTitle = GameObjectCreater::create("MenuToTitle")->componentManager().getComponent<MenuToTitle>();
    const auto& gameEnd = GameObjectCreater::create("MenuGameEnd")->componentManager().getComponent<MenuGameEnd>();
    const auto& menuClose = GameObjectCreater::create("MenuClose")->componentManager().getComponent<MenuClose>();
    menuClose->setMenuWindow(menuWindow);

    menuWindow->addMenuButton(toStageSelect.get());
    menuWindow->addMenuButton(toTitle.get());
    menuWindow->addMenuButton(gameEnd.get());
    menuWindow->addMenuButton(menuClose.get());
}
