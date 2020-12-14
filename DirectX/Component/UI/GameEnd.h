#pragma once

#include "../Component.h"

class GameEnd : public Component {
public:
    GameEnd(GameObject& gameObject);
    ~GameEnd();
    virtual void start() override;

private:
    GameEnd(const GameEnd&) = delete;
    GameEnd& operator=(const GameEnd&) = delete;
};
