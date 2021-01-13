#include "Hierarchy.h"
#include "../../../Device/Button.h"
#include "../../../Device/DrawString.h"
#include "../../../GameObject/GameObject.h"
#include "../../../Input/Input.h"
#include "../../../System/Window.h"
#include "../../../Transform/ParentChildRelationship.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/LevelLoader.h"
#include <string>

Hierarchy::Hierarchy() :
    mGameObjectsGetter(nullptr),
    mInspectorTargetSetter(nullptr),
    mNumRowsToDisplay(0),
    mLineSpace(0.f),
    mInspectorPositionX(0.f),
    mPosition(Vector2::zero),
    mScale(Vector2::one),
    mOffsetCharCountX(0),
    mOffsetCharCountY(0),
    mCharWidth(0.f),
    mCharHeight(0.f),
    mNonActiveAlpha(0.5f) {
}

Hierarchy::~Hierarchy() = default;

void Hierarchy::loadProperties(const rapidjson::Value& inObj) {
    const auto& obj = inObj["hierarchy"];
    if (obj.IsObject()) {
        JsonHelper::getVector2(obj, "scale", &mScale);
        JsonHelper::getInt(obj, "offsetCharCountX", &mOffsetCharCountX);
        JsonHelper::getInt(obj, "offsetCharCountY", &mOffsetCharCountY);
        JsonHelper::getFloat(obj, "lineSpace", &mLineSpace);
        JsonHelper::getFloat(obj, "nonActiveAlpha", &mNonActiveAlpha);
    }
    const auto& inspector = inObj["inspector"];
    if (inspector.IsObject()) {
        JsonHelper::getFloat(inspector, "inspectorPositionX", &mInspectorPositionX);
    }
}

void Hierarchy::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const {
    rapidjson::Value props(rapidjson::kObjectType);
    JsonHelper::setVector2(alloc, &props, "scale", mScale);
    JsonHelper::setInt(alloc, &props, "offsetCharCountX", mOffsetCharCountX);
    JsonHelper::setInt(alloc, &props, "offsetCharCountY", mOffsetCharCountY);
    JsonHelper::setFloat(alloc, &props, "lineSpace", mLineSpace);
    JsonHelper::setFloat(alloc, &props, "nonActiveAlpha", mNonActiveAlpha);

    inObj.AddMember("hierarchy", props, alloc);
}

void Hierarchy::initialize(const IGameObjectsGetter* getter, IInspectorTargetSetter* setter) {
    mGameObjectsGetter = getter;
    mInspectorTargetSetter = setter;

    mCharWidth = DrawString::WIDTH * mScale.x;
    mCharHeight = DrawString::HEIGHT * mScale.y;

    mPosition = Vector2(Window::width(), 0.f);
    mPosition += Vector2(mOffsetCharCountX * mCharWidth, mOffsetCharCountY * mCharHeight);

    //画面内に収まる行数
    mNumRowsToDisplay = (Window::debugHeight() - mPosition.y) / getOneLineHeight();

    mButtons.resize(mNumRowsToDisplay);
    auto pos = mPosition;
    for (auto&& b : mButtons) {
        //全ボタンに当たり判定をつける
        b.first = std::make_unique<Button>(nullptr, pos, Vector2(mInspectorPositionX - pos.x, mCharHeight));
        b.second = nullptr;
        downDrawPositionOneLine(pos);
    }
}

void Hierarchy::update() {
    setGameObjectToButton();

    if (!Input::mouse().getMouseButtonDown(MouseCode::LeftButton)) {
        return;
    }

    const auto& mousePos = Input::mouse().getMousePosition();
    for (const auto& b : mButtons) {
        if (!b.first->containsPoint(mousePos)) {
            continue;
        }
        if (b.second) {
            mInspectorTargetSetter->setTarget(b.second);
            break;
        }
    }
}

void Hierarchy::drawGameObjects(DrawString& drawString) const {
    auto pos = mPosition;
    for (const auto& b : mButtons) {
        const auto& obj = b.second;
        //オブジェクトが登録されてなかったら終了
        if (!obj) {
            break;
        }
        //親がいる場合は親に任せる
        if (haveParent(*obj)) {
            continue;
        }

        draw(drawString, *obj, pos);
        downDrawPositionOneLine(pos);

        drawChildren(drawString, *obj, pos);
    }
}

void Hierarchy::setGameObjectsGetter(const IGameObjectsGetter* getter) {
    mGameObjectsGetter = getter;
}

void Hierarchy::setGameObjectToButton() {
    for (auto&& b : mButtons) {
        b.second.reset();
    }

    const auto& gameObjects = mGameObjectsGetter->getGameObjects();
    auto itr = mButtons.begin();
    for (const auto& obj : gameObjects) {
        //親がいる場合は親に任せる
        if (haveParent(*obj)) {
            continue;
        }
        entryToButton(itr, obj);
        setGameObjectToButtonChildren(itr, obj);
    }
}

void Hierarchy::entryToButton(ButtonGameObjectPairList::iterator& itr, const GameObjectPtr& target) {
    //オブジェクトの数がボタンの数より多いときは無視
    if (itr == mButtons.end()) {
        return;
    }
    itr->second = target;
    ++itr;
}

void Hierarchy::setGameObjectToButtonChildren(ButtonGameObjectPairList::iterator& itr, const GameObjectPtr& parent) {
    const auto& children = parent->transform().getParentChildRelation().getChildren();
    for (const auto& child : children) {
        entryToButton(itr, child);
        setGameObjectToButtonChildren(itr, child);
    }
}

void Hierarchy::draw(DrawString& drawString, const GameObject& target, const Vector2& position, int childHierarchy) const {
    //アクティブ状態によって透明度を下げる
    float alpha = (target.getActive()) ? 1.f : mNonActiveAlpha;

    constexpr char CHILD_SPACE[] = " ";
    std::string space(CHILD_SPACE, childHierarchy * 2);
    drawString.drawString(space + target.name(), position, mScale, ColorPalette::white, alpha);
}

void Hierarchy::drawChildren(DrawString& drawString, const GameObject& parent, Vector2& position, int childHierarchy) const {
    const auto& children = parent.transform().getParentChildRelation().getChildren();
    for (const auto& child : children) {
        draw(drawString, *child, position, childHierarchy);
        downDrawPositionOneLine(position);

        drawChildren(drawString, *child, position, childHierarchy + 1);
    }
}

void Hierarchy::downDrawPositionOneLine(Vector2& position) const {
    position.y += getOneLineHeight();
}

float Hierarchy::getOneLineHeight() const {
    return (mCharHeight + mLineSpace);
}

bool Hierarchy::haveParent(const GameObject& gameObject) const {
    return (gameObject.transform().getParentChildRelation().parent());
}
