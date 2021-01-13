#pragma once

#include "../../Component.h"
#include <functional>
#include <memory>

class ASAI;
class Subject;

//アクションフェーズでのキャラクター管轄クラス
class CharacterAction : public Component {
public:
    CharacterAction(GameObject& gameObject);
    ~CharacterAction();
    virtual void start() override;
    virtual void update() override;
    virtual void onEnable(bool value) override;

    //このクラスをアクティブ化する
    void enabled();
    //このクラスを非アクティブ化する
    void disabled();
    //アクションフェーズに移行する際のコールバック
    void callbackChangeActionPhase(const std::function<void()>& callback);
    //操作フェーズに移行する際のコールバック
    void callbackChangeOperatePhase(const std::function<void()>& callback);

private:
    CharacterAction(const CharacterAction&) = delete;
    CharacterAction& operator=(const CharacterAction&) = delete;

private:
    std::shared_ptr<ASAI> mAI;
    std::unique_ptr<Subject> mCallbackChangeActionPhase;
    std::unique_ptr<Subject> mCallbackChangeOperatePhase;
    //このクラスが実行可能状態か
    bool mIsActive;
};
