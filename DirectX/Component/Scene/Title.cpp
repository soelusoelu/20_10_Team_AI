﻿#include "Title.h"
#include "Scene.h"
#include "../../DebugLayer/Debug.h"
#include "../../GameObject/GameObjectFactory.h"
#include "../../Input/Input.h"

Title::Title(GameObject& gameObject) :
    Component(gameObject),
    mScene(nullptr) {
}

Title::~Title() = default;

void Title::start() {
    mScene = getComponent<Scene>();
    GameObjectCreater::create("Ground");
    GameObjectCreater::create("CharacterCreater");
}

void Title::update() {
    if (Input::keyboard().getEnter()) {
        mScene->next("GamePlay");
    }

    Debug::renderLine(Vector3::left * 100.f, Vector3::right * 100.f, ColorPalette::red);
    Debug::renderLine(Vector3::down * 100.f, Vector3::up * 100.f, ColorPalette::green);
    Debug::renderLine(Vector3::back * 100.f, Vector3::forward * 100.f, ColorPalette::blue);
}
