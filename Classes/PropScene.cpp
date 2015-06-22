//
//  PropScene.cpp
//  mcdrunning
//
//  Created by chenbingfeng on 15/6/23.
//
//

#include "PropScene.h"
#include "StartScene.h"
#include "GoldLayer.h"
#include "GameState.h"
#include "format.h"
#include "Msg.h"
#include "uiconf.h"

USING_NS_CC;

bool PropScene::init()
{
    assert(TRBaseScene::init());

    addCommonBg();
    addCommonBackBtn([](){Director::getInstance()->replaceScene(StartScene::createScene());});
    addChild(GoldLayer::create());

    for (int i = 0; i < Prop::TMAX; i++) {
        auto p = GameState::s()->getProp(i);
        auto imgSp = Sprite::create(fmt::sprintf("images/prop/%s.png", p->getTitle()));
        imgSp->setPosition(genPos({0.15f, 0.1f + 0.15f*i}));
        imgSp->setScale(0.1f);
        _defaultLayer->addChild(imgSp);

        auto lbRealTitle = Label::createWithTTF(Msg::s()[p->getTitle()], uic::font_zh, uic::font_size);
        lbRealTitle->setPosition(genPos({0.15f, 0.16f + 0.15f*i}));
        lbRealTitle->setColor(Color3B::GRAY);
        _defaultLayer->addChild(lbRealTitle);

        std::string title = "";
        if (p->canUpgrade()) {
            title += fmt::sprintf("lv%d->lv%d Gold%d", p->level, p->level+1, p->upgradeCost());
        } else {
            title += Msg::s()["no_upgrade"];
        }

        auto lbTitle = Label::createWithTTF(title, uic::font_zh, 30);
        lbTitle->setPosition(genPos({0.5f, 0.16f + 0.15f*i}));
        _defaultLayer->addChild(lbTitle);
        _titleLbMap[i] = lbTitle;

        auto lbDesc = Label::createWithTTF(Msg::s()[fmt::sprintf("%s_desc", p->getTitle())], uic::font_zh, 20);
        lbDesc->setPosition(genPos({0.5f, 0.1f + 0.15f*i}));
        _defaultLayer->addChild(lbDesc);

        if (i != Prop::SHIELD) {
            addCommonBtn({0.85f, 0.1f + 0.15f*i}, Msg::s()["upgrade"], [this,i](){this->upgrade(i);});
        }
    }

    return true;
}

void PropScene::upgrade(int type)
{
    assert(type != Prop::SHIELD);
    auto p = GameState::s()->getProp(type);
    if (p->canUpgrade() && GameState::s()->getGold() >= p->upgradeCost()) {
        GameState::s()->addGold(0-p->upgradeCost());
        p->level++;

        std::string title = Msg::s()[p->getTitle()];
        if (p->canUpgrade()) {
            title += fmt::sprintf(" lv%d->lv%d Gold%d", p->level, p->level+1, p->upgradeCost());
        }

        _titleLbMap[type]->setString(title);
    }
}