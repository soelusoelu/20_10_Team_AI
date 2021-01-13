#pragma once

#include "IAssetsRenderTexturesGetter.h"
#include "ICallbackSelectAssetsTexture.h"
#include <functional>
#include <memory>

class Subject;

//アセットテクスチャ選択クラス
class AssetsTexturesSelector : public ICallbackSelectAssetsTexture {
public:
    AssetsTexturesSelector();
    ~AssetsTexturesSelector();
    virtual void callbackSelectTexture(const std::function<void()>& callback) override;

    void initialize(const IAssetsRenderTexturesGetter* getter);
    //マウスでテクスチャを選択する
    bool selectTexture(AssetsRenderTexturePtr& out);

private:
    AssetsTexturesSelector(const AssetsTexturesSelector&) = delete;
    AssetsTexturesSelector& operator=(const AssetsTexturesSelector&) = delete;

private:
    const IAssetsRenderTexturesGetter* mTexturesGetter;
    std::unique_ptr<Subject> mCallbackSelectTexture;
};
