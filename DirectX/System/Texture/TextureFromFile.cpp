#include "TextureFromFile.h"
#include "../../DebugLayer/Debug.h"
#include "../../DirectX/DirectXInclude.h"
#include "../../System/SystemInclude.h"
#include <DirectXTex/DirectXTex.h>

TextureFromFile::TextureFromFile(const std::string& filePath) :
    Texture() {
    createTextureFromFileName(filePath);
}

TextureFromFile::~TextureFromFile() = default;

void TextureFromFile::createTextureFromFileName(const std::string& filePath) {
    //DirectX::TexMetadata metadata{};
    //DirectX::ScratchImage scratchImage{};

    ////Unicodeへ変換する
    //wchar_t dst[256];
    //MultiByteToWideChar(CP_ACP, 0, filePath.c_str(), -1, dst, _countof(dst));

    ////ファイルからテクスチャ情報を取得
    //if (FAILED(DirectX::LoadFromWICFile(dst, DirectX::WIC_FLAGS_NONE, &metadata, scratchImage))) {
    //    Debug::windowMessage(filePath + ": テクスチャ読み込み失敗");
    //    return;
    //}

    ////生データ抽出
    //const DirectX::Image* image = scratchImage.GetImage(0, 0, 0);

    ////テクスチャサイズの設定
    //mTextureSize = Vector2(metadata.width, metadata.height);

    //ID3D11ShaderResourceView* srv = nullptr;
    //if (FAILED(DirectX::CreateShaderResourceView(MyDirectX::DirectX::instance().device(), image, 1, metadata, &srv))) {
    //    Debug::windowMessage(filePath + ": テクスチャ作成失敗");
    //}
    //mShaderResourceView = std::make_unique<ShaderResourceView>(srv);



    //ファイルからテクスチャ情報を取得
    D3DX11_IMAGE_INFO info{};
    D3DX11GetImageInfoFromFileA(filePath.c_str(), nullptr, &info, nullptr);

    mTextureSize = Vector2(info.Width, info.Height);

    D3DX11_IMAGE_LOAD_INFO loadInfo{};
    loadInfo.Width = info.Width;
    loadInfo.Height = info.Height;
    loadInfo.Depth = info.Depth;
    loadInfo.MipLevels = info.MipLevels;
    loadInfo.MiscFlags = info.MiscFlags;
    loadInfo.Format = info.Format;
    loadInfo.FirstMipLevel = 0;
    loadInfo.Usage = toUsage(Usage::USAGE_DEFAULT);
    loadInfo.BindFlags = static_cast<unsigned>(Texture2DBind::TEXTURE_BIND_SHADER_RESOURCE);
    loadInfo.CpuAccessFlags = 0;
    loadInfo.Filter = D3DX11_FILTER_LINEAR; //線形補間
    loadInfo.MipFilter = D3DX11_FILTER_LINEAR; //線形補間
    loadInfo.pSrcInfo = nullptr;

    ID3D11ShaderResourceView* srv = nullptr;
    if (FAILED(D3DX11CreateShaderResourceViewFromFileA(MyDirectX::DirectX::instance().device(), filePath.c_str(), &loadInfo, nullptr, &srv, nullptr))) {
        Debug::windowMessage(filePath + ": テクスチャ作成失敗");
    }
    mShaderResourceView = std::make_unique<ShaderResourceView>(srv);
}
