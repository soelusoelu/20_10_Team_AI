#include "EnemyCreater.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectFactory.h"
#include "../../DebugLayer/Debug.h"
#include "../../Transform/Transform3D.h"
#include "../../Utility/LevelLoader.h"
#include "../../Utility/StringUtil.h"

EnemyCreater::EnemyCreater(GameObject& gameObject)
    : Component(gameObject)
{
}

EnemyCreater::~EnemyCreater() = default;

void EnemyCreater::createEnemys(int stageNo) {
    rapidjson::Document doc;
    if (!LevelLoader::loadJSON(doc, "Enemys.json")) {
        return;
    }
    if (!doc.IsObject()) {
        return;
    }

    auto selectStage = "Stage" + StringUtil::intToString(stageNo);
    const auto& enemyArray = doc[selectStage.c_str()];
    if (!enemyArray.IsArray()) {
        return;
    }

    std::string type;
    Vector3 pos;
    for (rapidjson::SizeType i = 0; i < enemyArray.Size(); ++i) {
        const auto& enemy = enemyArray[i];
        if (!enemy.IsObject()) {
            continue;
        }

        //必要な情報を取得する
        JsonHelper::getString(enemy, "type", &type);
        JsonHelper::getVector3(enemy, "position", &pos);

        //エネミーを生成する
        auto e = GameObjectCreater::create(type);

        //エネミーの位置と向きを設定する
        auto& t = e->transform();
        t.setPosition(pos);
        t.rotate(Vector3::up, 180.f);

        //リストに登録する
        mEnemys.emplace_back(e);
    }
}
