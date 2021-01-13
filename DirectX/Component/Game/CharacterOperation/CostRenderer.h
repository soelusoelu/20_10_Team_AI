#pragma once

#include "../../Component.h"
#include <memory>
#include <string>

class CharacterCost;
class Text;

//キャラクターのコストを描画するクラス
class CostRenderer : public Component {
public:
    CostRenderer(GameObject& gameObject);
    ~CostRenderer();
    virtual void start() override;

private:
    CostRenderer(const CostRenderer&) = delete;
    CostRenderer& operator=(const CostRenderer&) = delete;

    //表示する文字列を作成する
    std::string createText(int currentCost) const;
    //コストが更新されたら
    void onUpdateCost();

private:
    std::shared_ptr<CharacterCost> mCost;
    std::shared_ptr<Text> mText;
};
