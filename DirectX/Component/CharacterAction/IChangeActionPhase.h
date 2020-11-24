#pragma once

//アクションフェーズに移行する際のコールバックインターフェース
class IChangeActionPhase {
public:
    virtual ~IChangeActionPhase() = default;
    //アクションフェーズに変わった際の処理
    virtual void onChangeActionPhase() = 0;
};