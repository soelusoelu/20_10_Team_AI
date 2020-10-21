#pragma once

#include "Texture.h"
#include "../DirectX/TextureDesc.h"
#include "../System/SystemInclude.h"
#include <string>

class TextureFromFile : public Texture {
public:
    TextureFromFile(const std::string& fileName);
    ~TextureFromFile();

private:
    TextureFromFile(const TextureFromFile&) = delete;
    TextureFromFile& operator=(const TextureFromFile&) = delete;

    //ファイル名からテクスチャを作成する
    void createTextureFromFileName(const std::string& fileName);
    //各種変換
    D3DX11_IMAGE_LOAD_INFO toImageLoadInfo(const TextureDesc& desc) const;
    unsigned toFilter(TextureFilter filter) const;
};
