#pragma once

#include "../Map/IMap.h"
#include <list>
#include <memory>

class CharacterCommonComponents;

using CharacterPtr = std::shared_ptr<CharacterCommonComponents>;
using CharacterPtrList = std::list<CharacterPtr>;

//全キャラクター管理インターフェース
class ICharacterManager {
public:
    virtual ~ICharacterManager() = default;
    //全キャラクターを取得する
    virtual const CharacterPtrList& getCharacters() const = 0;
    //全エネミーを取得する
    virtual const CharacterPtrList& getEnemys() const = 0;
    //マップ情報を取得する
    virtual const IMap& getMap() const = 0;
};