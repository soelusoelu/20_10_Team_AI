#pragma once

#include "../Component.h"
#include <memory>

class Sprite3D;

//HP表示クラス
class HitPointRenderer : public Component {
public:
    HitPointRenderer(GameObject& gameObject);
    ~HitPointRenderer();
    virtual void start() override;

private:
    HitPointRenderer(const HitPointRenderer&) = delete;
    HitPointRenderer& operator=(const HitPointRenderer&) = delete;

private:
    std::shared_ptr<Sprite3D> mSprite;
};
