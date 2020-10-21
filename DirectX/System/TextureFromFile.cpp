#include "TextureFromFile.h"
#include "../DebugLayer/Debug.h"
#include "../DirectX/DirectXInclude.h"

TextureFromFile::TextureFromFile(const std::string& fileName) :
    Texture() {
    createTextureFromFileName(fileName);
}

TextureFromFile::~TextureFromFile() = default;

void TextureFromFile::createTextureFromFileName(const std::string& fileName) {
    //ファイルからテクスチャ情報を取得
    D3DX11_IMAGE_INFO info;
    D3DX11GetImageInfoFromFileA(fileName.c_str(), nullptr, &info, nullptr);

    mDesc.width = info.Width;
    mDesc.height = info.Height;

    ID3D11ShaderResourceView* srv = nullptr;
    if (FAILED(D3DX11CreateShaderResourceViewFromFileA(DirectX::instance().device(), fileName.c_str(), &toImageLoadInfo(mDesc), nullptr, &srv, nullptr))) {
        Debug::windowMessage(fileName + ": テクスチャ作成失敗");
    }
    mShaderResourceView = std::make_unique<ShaderResourceView>(srv);
}

D3DX11_IMAGE_LOAD_INFO TextureFromFile::toImageLoadInfo(const TextureDesc& desc) const {
    D3DX11_IMAGE_LOAD_INFO info;
    info.Width = desc.width;
    info.Height = desc.height;
    info.Depth = desc.depth;
    info.FirstMipLevel = desc.firstMipLevel;
    info.MipLevels = desc.mipLevels;
    info.Usage = toUsage(desc.usage);
    info.BindFlags = desc.bindFlags;
    info.CpuAccessFlags = desc.cpuAccessFlags;
    info.MiscFlags = desc.miscFlags;
    info.Format = toFormat(desc.format);
    info.Filter = toFilter(desc.filter);
    info.MipFilter = toFilter(desc.mipFilter);
    info.pSrcInfo = desc.srcInfo;

    return info;
}

unsigned TextureFromFile::toFilter(TextureFilter filter) const {
    const unsigned filters[] = {
        D3DX11_FILTER_POINT,
        D3DX11_FILTER_LINEAR,
        D3DX11_FILTER_TRIANGLE,
    };
    return filters[static_cast<unsigned>(filter)];
}
