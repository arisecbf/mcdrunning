//
//  SpeedLayer.cpp
//  mcdrunning
//
//  Created by chenbingfeng on 15/6/23.
//
//

#include "SpeedLayer.h"
#include "uiconf.h"
#include "format.h"
#include "GameState.h"
USING_NS_CC;

static const float length2scale = 0.06f;

bool SpeedLayer::init(bool showIndexBar)
{
    assert(McdLayer::init());

    auto mainSp = Sprite::create("images/speed.png");
    mainSp->setPosition(genPos({0.5f, 0.80f}));
    mainSp->setScale(GameState::s()->getSpeedBarLength()*length2scale, 1.f);
    this->addChild(mainSp);
    _mainSp = mainSp;

    auto updateFunc = [this](float dt){
        this->_mainSp->setScale(GameState::s()->getSpeedBarLength()*length2scale, 1.f);
    };

    schedule(updateFunc, 0.333f, kRepeatForever, 0, "spp");

    return true;
}