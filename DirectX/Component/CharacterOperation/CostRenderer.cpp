#include "CostRenderer.h"
#include "CharacterCost.h"
#include "../Text/Text.h"
#include "../../Utility/StringUtil.h"

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
    auto beginText = StringUtil::intToString(mCost->getMaxCost());
    mText->setText(beginText + "/" + beginText);

    //コールバック登録
    mCost->callbackUpdateCost([this] { onUpdateCost(); });
}

void CostRenderer::onUpdateCost() {
    mText->setText(StringUtil::intToString(mCost->getCost()) + "/" + StringUtil::intToString(mCost->getMaxCost()));
}
