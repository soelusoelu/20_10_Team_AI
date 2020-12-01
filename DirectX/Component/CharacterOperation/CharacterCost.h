#pragma once

#include "../Component.h"
#include <functional>
#include <memory>

class Subject;

//キャラクター生成コスト管理クラス
class CharacterCost : public Component {
public:
    CharacterCost(GameObject& gameObject);
    ~CharacterCost();
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawInspector() override;
    //指定の量コストを使う
    void useCost(int amount);
    //指定の量のコストが戻ってくる
    void returnCost(int amount);
    //コストを設定する
    void setCost(int cost, bool maxToo = true);
    //残りのコストを取得する
    int getCost() const;
    //最大コストを取得する
    int getMaxCost() const;
    //コストが更新された際のコールバック
    void callbackUpdateCost(const std::function<void()>& callback);

private:
    CharacterCost(const CharacterCost&) = delete;
    CharacterCost& operator=(const CharacterCost&) = delete;

    //コストを適正範囲にクランプする
    void clampCost();

private:
    int mMaxCost;
    int mCurrentCost;
    std::unique_ptr<Subject> mCallbackUpdateCost;
};
