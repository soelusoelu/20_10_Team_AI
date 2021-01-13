#pragma once

#include "../../Component.h"
#include <functional>

class NextStageSelect : public Component {
public:
    NextStageSelect(GameObject& gameObject);
    ~NextStageSelect();
    virtual void start() override;
    void callbackButtonClick(const std::function<void()>& callback);

private:
    NextStageSelect(const NextStageSelect&) = delete;
    NextStageSelect& operator=(const NextStageSelect&) = delete;
};
