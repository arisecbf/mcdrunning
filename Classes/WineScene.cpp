//
//  WineScene.cpp
//  mcdrunning
//
//  Created by chenbingfeng on 15/6/23.
//
//

#include "WineScene.h"
#include "SpeedLayer.h"
#include "StartScene.h"
#include "Msg.h"
#include "format.h"
#include "GameState.h"

USING_NS_CC;


static const int max_speed_length = 100;
static const int min_speed_length = 60;
static const int add_cost = 250;
static const int minus_owe = 50;
static const int speed_length_step = 5;

bool WineScene::init()
{
    assert(TRBaseScene::init());

    addCommonBg();
    addCommonBackBtn([](){Director::getInstance()->replaceScene(StartScene::createScene());});

    this->addChild(SpeedLayer::create(false));
    this->addChild(GoldLayer::create());


    auto addFunc = [](){
        if (GameState::s()->getSpeedBarLength() < max_speed_length && GameState::s()->getGold() >= add_cost) {
            GameState::s()->setSpeedBarLength(GameState::s()->getSpeedBarLength() + speed_length_step);
            GameState::s()->setGold(GameState::s()->getGold() - add_cost);
        }
    };

    auto minusFunc = [](){
        if (GameState::s()->getSpeedBarLength() > min_speed_length ) {
            GameState::s()->setSpeedBarLength(GameState::s()->getSpeedBarLength() - speed_length_step);
            GameState::s()->setGold(GameState::s()->getGold() + minus_owe);
        }
    };

    // add drink
    addCommonBtn({0.3f, 0.5f}, Msg::s()["addWine"], addFunc);
    addCommonLabel({0.3f, 0.3f}, Msg::s()["addWine_desc"]);
    

    // minus drink
    addCommonBtn({0.7f, 0.5f}, Msg::s()["minusWine"], minusFunc);
    addCommonLabel({0.7f, 0.3f}, Msg::s()["minusWine_desc"]);

    return true;
}