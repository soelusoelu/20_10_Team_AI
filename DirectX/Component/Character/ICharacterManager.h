#pragma once

#include "../Map/IMap.h"

//全キャラクター管理インターフェース
class ICharacterManager {
public:
    virtual ~ICharacterManager() = default;
    //マップ情報を取得する
    virtual const IMap& getMap() const = 0;
};