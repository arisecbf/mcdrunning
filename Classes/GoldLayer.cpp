//
//  GoldLayer.cpp
//  mcdrunning
//
//  Created by chenbingfeng on 15/6/22.
//
//

#include "GoldLayer.h"
#include "format.h"
#include "GameState.h"
#include <cstdlib>
#include "SimpleAudioEngine.h"
#include "BuyGoldScene.h"

USING_NS_CC;


static const Btn_info add_btn_info = {"images/gold_add.png", "images/gold_add.png", {0.9, 0.9}, 3.f};
static const int gold_ani_step_max = 40;
static const float gold_ani_check_interval = 0.15f;

bool GoldLayer::init()
{
    return init(true);
}

bool GoldLayer::init(bool hasBtn)
{
    assert(McdLayer::init());

    _goldDisp = GameState::g()->getGold();

    auto sp = Sprite::create("images/gold_icon.png");
    sp->setPosition(genPos({0.58, 0.9}));
    sp->setScale(0.4f);
    this->addChild(sp);

    if (hasBtn) decorateButton(add_btn_info, 102);

    auto goldStr = fmt::sprintf("%d", _goldDisp);

    _lb = Label::createWithTTF("999", "fonts/caton.ttf", 60);
    _lb->setColor(Color3B::YELLOW);
    _lb->setPosition(genPos({0.76,0.9}));
    this->addChild(_lb);

    schedule(schedule_selector(GoldLayer::updateGold), gold_ani_check_interval, kRepeatForever, 0);

    return true;
}

GoldLayer* GoldLayer::create(bool hasBtn)
{
    auto ret = new GoldLayer();
    if (ret && ret->init(hasBtn)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void GoldLayer::onButtonClick(int tag)
{
    CCLOG("add gold click");
//    GameState::g()->setGold(rand()%1000);
    Director::getInstance()->replaceScene(BuyGoldScene::create());
}

void GoldLayer::updateGold(float dt)
{
    // 监视实际的gold量，进行数目变化及音效
    if (_goldDisp != GameState::g()->getGold()){

        // 小量的增减发生在游戏过程中，此时我们直接增加即可
        if (std::abs(_goldDisp - GameState::g()->getGold()) <= 10) {

            return;
        }

        // 大量的加减发生在充值或者花费金币场合，进行动画及音效
        _goldAniFrom = _goldDisp;
        _goldDisp = GameState::g()->getGold();
        _goldAniTo = _goldDisp;
        _goldAniIndex = 0;
        auto anifunc = [&](float dt){
            _goldAniIndex ++;
            int showGold = _goldAniIndex *1.f/gold_ani_step_max * (_goldAniTo - _goldAniFrom) + _goldAniFrom;
            if (_goldAniIndex >= (gold_ani_step_max-1) ) { showGold = _goldDisp;}
            _lb->setString(fmt::sprintf("%d",showGold));
        };
        schedule(anifunc, 0.033, gold_ani_step_max, 0, "gold ani");
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/gold_updown.mp3");

    }
}