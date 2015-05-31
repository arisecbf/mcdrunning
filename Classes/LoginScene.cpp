//
//  LoginScene.cpp
//  mcdrunning
//
//  Created by chenbingfeng on 15/5/31.
//
//

#include "LoginScene.h"
#include "Common.h"
#include "SimpleAudioEngine.h"
#include "StartScene.h"

USING_NS_CC;

Scene* LoginScene::createScene()
{
    auto scene = Scene::create();

    auto layer = LoginScene::create();

    scene->addChild(layer);
    
    return scene;
}
void LoginScene::onButtonClick(int tag)
{
    switch (tag) {
        case BTN_QQ:
            CCLOG("login btn qq");
            turnOut();
            scheduleOnce([](float){
                Director::getInstance()->replaceScene(StartScene::createScene());}, config::default_turn_out_time, "sdfd");
            break;
        case BTN_GUEST:
            CCLOG("login btn guest");
            break;
        default:
            break;
    }
}

bool LoginScene::init()
{
    if (!Layer::init()){
        return false;
    }
    Size windowsSize = Director::getInstance()->getVisibleSize();
    auto center =Vec2{windowsSize.width, windowsSize.height} * 0.5f;

    // bg
    auto bg = Sprite::create(config::login_bg);
    mcd::scale2size(bg, windowsSize);
    bg->setPosition(center);
    this->addChild(bg, 0);

    // bgm
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(config::bgm_default.c_str(), true);

    // buttons
    withInOut(decorateButton(config::login_btn_guest, BTN_GUEST));
    withInOut(decorateButton(config::login_btn_qq, BTN_QQ));

    turnIn();
    return true;
}