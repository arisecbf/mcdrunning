//
//  StartScene.cpp
//  mcdrunning
//
//  Created by chenbingfeng on 15/5/31.
//
//

#include "StartScene.h"
#include "Common.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* StartScene::createScene()
{
    auto scene = Scene::create();

    auto layer = StartScene::create();

    scene->addChild(layer);

    return scene;
}

bool StartScene::init()
{
    if (!McdLayer::init()){
        return false;
    }

    Size windowsSize = Director::getInstance()->getVisibleSize();
    auto center =Vec2{windowsSize.width, windowsSize.height} * 0.5f;

    // white bg
    auto bg = Sprite::create(config::start_bg);
    mcd::scale2size(bg, windowsSize);
    bg->setPosition(center);
    this->addChild(bg, 0);

    // bgm
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(config::bgm_default.c_str(), true);

    // buttons
    decorateButton(config::start_btn_coin, BTN_COIN);
    decorateButton(config::start_btn_head, BTN_HEAD);
    decorateButton(config::start_btn_role, BTN_ROLE);
    decorateButton(config::start_btn_asset, BTN_ASSET);
    decorateButton(config::start_btn_drink, BTN_DRINK);
    decorateButton(config::start_btn_setting, BTN_SETTING);
    decorateButton(config::start_btn_rank, BTN_RANK);
    decorateButton(config::start_btn_play, BTN_PLAY);

    // img
    decorateImg(config::start_role, config::start_img_role);

    return true;
}

void StartScene::onButtonClick(int tag)
{
    
}