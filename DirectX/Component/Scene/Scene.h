#pragma once

#include "../Component.h"
#include <string>
#include <unordered_map>
#include <utility>

using ValuePassMap = std::unordered_map<std::string, std::any>;

class Scene : public Component {
public:
    Scene(GameObject& gameObject);
    ~Scene();
    //初期化処理
    void initialize(const ValuePassMap& values);
    //設定した次のシーンに遷移
    void next(const std::string& next);
    //次のシーンの取得
    const std::string& getNext() const;
    //次のシーンに渡す値を設定する
    void addValuePassToNextScene(const std::string& valueName, const std::any& value);
    //次のシーンに渡す値を取得する
    const ValuePassMap& getValuePassToNextScene() const;
    //前のシーンから渡された値
    const std::any& getValueFromPreviousScene(const std::string& valueName) const;

private:
    std::string mNext;
    ValuePassMap mValuesPassToNextScene;
    ValuePassMap mValuesFromPreviousScene;
};
