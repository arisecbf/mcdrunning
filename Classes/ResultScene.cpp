//
//  ResultScene.cpp
//  mcdrunning
//
//  Created by chenbingfeng on 15/6/24.
//
//

#include "ResultScene.h"
#include "format.h"
#include "uiconf.h"
#include "StartScene.h"
#include "Msg.h"
#include "RunningScene.h"

USING_NS_CC;

ResultScene* ResultScene::create(float rawRoad, int cntProp, int cntMonster, float rawHighestSpeed, int cntGold)
{
    auto ret = new ResultScene();
    if (ret && ret->init(rawRoad, cntProp, cntMonster, rawHighestSpeed, cntGold)){
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ResultScene::init(float rawRoad, int cntProp, int cntMonster, float rawHighestSpeed, int cntGold)
{
    assert(TRBaseScene::init());

    addCommonBackBtn([](){Director::getInstance()->replaceScene(StartScene::createScene());});
    addCommonBg();
    addCommonBtn({0.5f,0.35f}, Msg::s()["again"],[](){Director::getInstance()->replaceScene(RunningScene::create());});

    addCommonLabel({0.5f,0.7f}, fmt::sprintf("rawRoad=%.3f\ncntProp=%d\n cntMonster=%d\n rawHighestSpeed=%.3f\n cntGold=%d ", rawRoad, cntProp, cntMonster, rawHighestSpeed, cntGold));

    // TODO server conn


    return true;
}