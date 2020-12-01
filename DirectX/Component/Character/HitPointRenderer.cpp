#include "HitPointRenderer.h"
#include "../Character/CharacterAction.h"
#include "../Character/PhaseChangeSaver.h"
#include "../Collider/AABBCollider.h"
#include "../Other/HitPointComponent.h"
#include "../Sprite/Sprite3D.h"
#include "../../Transform/Transform3D.h"
#include "../../Utility/LevelLoader.h"

HitPointRenderer::HitPointRenderer(GameObject& gameObject)
    : Component(gameObject)
    , mHp(nullptr)
    , mCollider(nullptr)
    , mCurrentHp(nullptr)
    , mLostHp(nullptr)
{
}

HitPointRenderer::~HitPointRenderer() = default;

void HitPointRenderer::awake() {
    //3Dスプライト追加
    mCurrentHp = addComponent<Sprite3D>("Sprite3D");
    //取得してあるファイル名からテクスチャを生成する
    mCurrentHp->setTextureFromFileName(mCurrentHpFileName);
    //アスペクト比からピボット位置を設定する
    const auto& aspect = mCurrentHp->getTextureAspect();
    float aspectRatioX = aspect.y / aspect.x;
    mCurrentHp->transform().setPivot(Vector3::down * 0.5f);
    //ビルボードで扱う
    mCurrentHp->setBillboard(true);
    //最初は使わない
    mCurrentHp->setActive(false);

    //上と同じ
    mLostHp = addComponent<Sprite3D>("Sprite3D");
    mLostHp->setTextureFromFileName(mLostHpFileName);
    mLostHp->transform().setPivot(mCurrentHp->transform().getPivot());
    mLostHp->setBillboard(mCurrentHp->isBillboard());
    mLostHp->setActive(mCurrentHp->getActive());
}

void HitPointRenderer::start() {
    //各種コンポーネント取得
    mHp = getComponent<HitPointComponent>();
    mCollider = getComponent<AABBCollider>();

    //コールバック登録
    mHp->callbackUpdateHP([&] { onUpdateHp(); });

    auto charaAction = getComponent<CharacterAction>();
    charaAction->callbackChangeActionPhase([&] { onChangeActionPhase(); });
    charaAction->callbackChangeOperatePhase([&] { onChangeOperatePhase(); });
}

void HitPointRenderer::update() {
    const auto& aabb = mCollider->getAABB();
    const auto& renderPos = Vector3(
        (aabb.max.x + aabb.min.x) / 2.f,
        aabb.max.y,
        (aabb.max.z + aabb.min.z) / 2.f
    );

    mCurrentHp->transform().setPosition(renderPos);
    mLostHp->transform().setPosition(renderPos);
}

void HitPointRenderer::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getString(inObj, "hpBar", &mCurrentHpFileName);
    JsonHelper::getString(inObj, "lostHpBar", &mLostHpFileName);
}

void HitPointRenderer::onChangeActionPhase() {
    mCurrentHp->setActive(true);
    mLostHp->setActive(true);
}

void HitPointRenderer::onChangeOperatePhase() {
    mCurrentHp->setActive(false);
    mLostHp->setActive(false);
}

void HitPointRenderer::onUpdateHp() {
    //HPの残りの割合
    float ratio = mHp->hpRate();

    //HPが0なら用はない
    if (mHp->getHP() == 0) {
        if (mCurrentHp->getActive()) {
            mCurrentHp->setActive(false);
        }
    } else {
        mCurrentHp->setUV(0.f, 0.f, ratio, 1.f);
    }

    //HPが最大なら用はない
    if (mHp->getHP() == mHp->getMaxHP()) {
        if (mLostHp->getActive()) {
            mLostHp->setActive(false);
        }
        return;
    }

    if (!mLostHp->getActive()) {
        mLostHp->setActive(true);
    }
    mLostHp->setUV(ratio, 0.f, 1.f, 1.f);
}
