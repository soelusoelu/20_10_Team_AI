#pragma once

#include "CharacterCreateInfo.h"
#include "ICharacterCreateInfo.h"
#include "../../Component.h"
#include "../../../Math/Math.h"
#include <memory>
#include <vector>

class CharacterCreateSpriteOperation;
class CharacterCost;

//キャラクターを生成するクラス
class CharacterCreater : public Component, public ICharacterCreateInfo {
public:
    CharacterCreater(GameObject& gameObject);
    ~CharacterCreater();
    virtual void start() override;

    virtual const CharacterCreateInfo& getCharacterCreateInfo(unsigned index) const override;
    virtual unsigned getCharacterCount() const override;

    //コンポーネント外のアップデート
    void originalUpdate();
    //ゲームオブジェクトを生成する
    void create(std::shared_ptr<GameObject>& out, int& cost);
    //外部からデータを受け取る
    void receiveExternalData(const rapidjson::Value& inObj);
    //このクラスを操作中か
    bool isOperating() const;

private:
    CharacterCreater(const CharacterCreater&) = delete;
    CharacterCreater& operator=(const CharacterCreater&) = delete;

private:
    std::shared_ptr<CharacterCreateSpriteOperation> mSpriteOperator;
    std::shared_ptr<CharacterCost> mCost;
    //キャラクター配列
    std::vector<CharacterCreateInfo> mCharactersInfo;
};
