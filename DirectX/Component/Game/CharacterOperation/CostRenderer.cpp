#include "CostRenderer.h"
#include "CharacterCost.h"
#include "../../Engine/Text/Text.h"
#include "../../../Utility/StringUtil.h"

CostRenderer::CostRenderer(GameObject& gameObject)
    : Component(gameObject)
    , mCost(nullptr)
    , mText(nullptr)
{
}

CostRenderer::~CostRenderer() = default;

void CostRenderer::start() {
    mCost = getComponent<CharacterCost>();
    mText = getComponent<Text>();

    //初期テキストを設定する
    mText->setText(createText(mCost->getMaxCost()));

    //コールバック登録
    mCost->callbackUpdateCost([this] { onUpdateCost(); });
}

std::string CostRenderer::createText(int currentCost) const {
    return (StringUtil::intToString(currentCost) + "/" + StringUtil::intToString(mCost->getMaxCost()));
}

void CostRenderer::onUpdateCost() {
    mText->setText(createText(mCost->getCost()));
}
