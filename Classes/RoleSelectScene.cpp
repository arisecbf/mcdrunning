//
//  RoleSelectScene.cpp
//  mcdrunning
//
//  Created by chenbingfeng on 15/5/31.
//
//

#include "RoleSelectScene.h"
#include "Common.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* RoleSelectScene::createScene()
{
    auto scene = Scene::create();

    auto layer = RoleSelectScene::create();

    scene->addChild(layer);

    return scene;
}

bool RoleSelectScene::init()
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
    withInOut(decorateButton(config::start_btn_coin, BTN_COIN));
    withInOut(decorateButton(config::role_btn_select, BTN_SELECT));
    withInOut(decorateButton(config::role_btn_prev, BTN_PREV));
    withInOut(decorateButton(config::role_btn_next, BTN_NEXT));

    for (int i = 0; i < config::role_vec_imgs.size(); i++){
        auto img = config::role_vec_imgs[i];
        auto sp = Sprite::create(img);
        this->_roles.push_back(sp);
        sp->setPosition(center);
        this->addChild(sp);
    }
    animate2index(_index);

    turnIn();
    return true;
}

void RoleSelectScene::animate2index(int index)
{
    for (int i = 0; i < _roles.size(); i++) {
        int rel = i - index;
        auto sp = _roles[i];
        int z = 100 - abs(rel);
        sp->setZOrder(z);
        float scale = powf(config::role_scale_step, abs(rel));
        auto actScale = ScaleTo::create(config::role_interval, scale, scale);
        Vec2 newPos = config::role_pos_middle + rel * config::role_pos_step;
        auto actPos = MoveTo::create(config::role_interval, Director::getInstance()->getVisibleSize() * newPos);
        sp->runAction(Spawn::create(actPos, actScale, NULL));
    }
}

void RoleSelectScene::onButtonClick(int tag)
{
    switch (tag) {
        case BTN_PREV:
            CCLOG("role btn prev");
            if (_index != 0) _index--;
            animate2index(_index);
            break;

        case BTN_NEXT:
            CCLOG("role btn next");
            if (_index != _roles.size() - 1) _index++;
            animate2index(_index);
            break;

        default:
            break;
    }
}