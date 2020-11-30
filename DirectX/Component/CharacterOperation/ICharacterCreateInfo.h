#pragma once

#include "CharacterCreateInfo.h"
#include <vector>

//キャラクター生成情報インターフェース
class ICharacterCreateInfo {
public:
    virtual ~ICharacterCreateInfo() = default;
    //キャラクター情報を取得する
    virtual const CharacterCreateInfo& getCharacterCreateInfo(unsigned index) const = 0;
    //キャラクター総数を取得する
    virtual unsigned getCharacterCount() const = 0;
};
