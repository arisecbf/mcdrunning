//
//  BuyGoldScene.cpp
//  mcdrunning
//
//  Created by chenbingfeng on 15/6/22.
//
//

#include "BuyGoldScene.h"
#include "format.h"
#include "uiconf.h"
#include "GoldLayer.h"
#include "GameState.h"
// 1000 金币售价 1 元,10000 金币售价 5 元,50000 金币售10元
USING_NS_CC;

bool BuyGoldScene::init()
{
    assert(TRBaseScene::init());

    // bg
    addCommonBg();

    //gold layer
    this->addChild(GoldLayer::create(false));

    // 3个宝箱图片
    auto addBoxFunc = [&](Vec2 pos)
    {
        auto sp = Sprite::create("images/jingbi_box.png");
        sp->setPosition(this->genPos(pos));
        sp->setScale(0.5f);
        _defaultLayer->addChild(sp);
    };

    auto addTexFunc = [&](Vec2 pos, int goldNum)
    {
        auto str = fmt::sprintf("%d", goldNum);
        auto lb = Label::createWithTTF(str, uic::font_en, uic::font_size);
        lb->setPosition(this->genPos(pos));
        _defaultLayer->addChild(lb);
    };

    addBoxFunc({0.3f, 0.75f});
    addBoxFunc({0.3f, 0.5f});
    addBoxFunc({0.3f, 0.25f});
    addTexFunc({0.3f, 0.7f}, 1000);
    addTexFunc({0.3f, 0.45f}, 10000);
    addTexFunc({0.3f, 0.2f}, 50000);

    _defaultLayer->decorateButtonEx({"images/gold_buy.png", "images/gold_buy.png", {0.8f,0.75f}, .5f}, [](){CCLOG("buy 1k"); GameState::k()->addGold(1000); });
    _defaultLayer->decorateButtonEx({"images/gold_buy.png", "images/gold_buy.png", {0.8f,0.5f}, .5f}, [](){CCLOG("buy 10k"); GameState::k()->addGold(10000); });
    _defaultLayer->decorateButtonEx({"images/gold_buy.png", "images/gold_buy.png", {0.8f,0.25f}, .5f}, [](){CCLOG("buy 50k"); GameState::k()->addGold(50000); });
    return  true;
}