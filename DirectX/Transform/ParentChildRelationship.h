#pragma once

#include <list>
#include <memory>

class GameObject;
class Transform3D;

class ParentChildRelationship {
    using Parent = ParentChildRelationship*;
    using Child = std::shared_ptr<GameObject>;
    using Children = std::list<Child>;

public:
    ParentChildRelationship(Transform3D* transform);
    ~ParentChildRelationship();

    //子を追加する
    void addChild(const Child& child);
    //子を削除する
    void removeChild(const Child& child);
    //子をすべて取得する
    const Children& getChildren() const;
    //親を取得する
    Parent parent() const;
    //一番上の親を取得する
    Parent root() const;
    //子の数を取得する
    size_t getChildCount() const;
    //Transform3Dを取得する
    Transform3D& transform() const;

private:
    ParentChildRelationship(const ParentChildRelationship&) = delete;
    ParentChildRelationship& operator=(const ParentChildRelationship&) = delete;

    //親を設定する
    void setParent(const Parent& parent);

private:
    Transform3D* mTransform;
    Parent mParent;
    Children mChildren;
};
