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

void LoginScene::touchEvent(Ref *pSender, cocos2d::ui::Widget::TouchEventType type){

    auto button = static_cast<cocos2d::ui::Button*>(pSender);
    int tag = button->getTag();
    if (type != cocos2d::ui::Widget::TouchEventType::ENDED) {
        return;
    }
    switch (tag) {
        case BTN_QQ:
            CCLOG("login btn qq");
            Director::getInstance()->replaceScene(StartScene::createScene());
            break;
        case BTN_GUEST:
            CCLOG("login btn guest");
            break;
        default:
            break;
    }
}

void LoginScene::decorateButton(const config::Btn_info& btnInfo , int tag)
{
    auto button = ui::Button::create(btnInfo.img, btnInfo.img_p);
    button->setPressedActionEnabled(true);
//    button->setAnchorPoint({0,0});
    auto size = Director::getInstance()->getVisibleSize();
    button->setPosition(btnInfo.pos * size);
    button->setTag(tag);
    button->addTouchEventListener(CC_CALLBACK_2(LoginScene::touchEvent, this));
//    button->setScale(config::btn_scale);
    this->addChild(button, 1);
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
    decorateButton(config::login_btn_guest, BTN_GUEST);
    decorateButton(config::login_btn_qq, BTN_QQ);
    return true;
}