#pragma once

#include <memory>
#include <string>

//キャラクター生成時情報
struct CharacterCreateInfo {
    std::string fileName;
    std::string spriteFileName;
    int cost;
};
