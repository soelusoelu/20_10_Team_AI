#pragma once

#include "../Component.h"
#include <memory>

class ASAI;

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

private:
    CharacterAction(const CharacterAction&) = delete;
    CharacterAction& operator=(const CharacterAction&) = delete;

private:
    std::shared_ptr<ASAI> mAI;
    //このクラスが実行可能状態か
    bool mIsActive;
};
