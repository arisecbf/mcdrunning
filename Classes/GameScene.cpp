//
//  GameScene.cpp
//  mcdrunning
//
//  Created by chenbingfeng on 15/5/31.
//
//

#include "GameScene.h"

USING_NS_CC;
bool GameScene::init()
{
    if (!Scene::init()){
        return false;
    }
    CCLOG("game scene init");
    _bgLayer = Layer::create();
    this->addChild(_bgLayer, 0);

    auto size = Director::getInstance()->getVisibleSize();
    Vec2 center = {size.width/2, size.height/2};


    // 3D layer
    _gameLayer = Layer::create();
    this->addChild(_gameLayer, 1);

    auto bg =  BillBoard::create("images/street_bg.jpg");
    bg->setPosition3D({0,0,100});
    bg->setScale(4.f);
    _gameLayer->addChild(bg);
    auto sp3d = Sprite3D::create("3d/street_wall.c3b");
    sp3d->setPosition3D({0, 0, 900});
    _gameLayer->addChild(sp3d, 1);
    sp3d->setScale(5.f);
    auto sp3d2 = Sprite3D::create("3d/street_wall.c3b");
    _gameLayer->addChild(sp3d2, 1);
    sp3d2->setPosition3D({0, 0, 725});
    sp3d2->setScale(5.f);
    _streets.push_back(sp3d);
    _streets.push_back(sp3d2);
    // camera
    _camera = Camera::createPerspective(60, size.width/size.height, 1, 1000);
    _camera->setPosition3D({0, 0, 1000});
    _camera->lookAt({0,0,0});
    _camera->setCameraFlag(CameraFlag::USER1);
    _gameLayer->addChild(_camera);

    _gameLayer->setCameraMask((unsigned short)CameraFlag::USER1);

//    auto ac = MoveTo::create(10.f, {0,0,1000});
//    sp3d->runAction(ac);

    // front UI layer
    _uiLayer = Layer::create();
    this->addChild(_uiLayer, 2);

    // speed bar
    auto speedBar = Sprite::create("images/speed_bar.png");
    speedBar->setPosition(Vec2{0.5f, 0.85f} * size);
    speedBar->setScale(2.5f, 2.0f);
    _uiLayer->addChild(speedBar, 100);

    // score labels
    _lbLength = Label::createWithTTF("0", "fonts/caton.ttf", 40);
    _lbScore = Label::createWithTTF("0", "fonts/caton.ttf", 40);
    _lbLength->setPosition(Vec2{0.25f, 0.9f} * size);
    _lbScore->setPosition(Vec2{0.75f, 0.9f} * size);
    _lbLength->setColor({0,0,0});
    _lbScore->setColor({0,0,0});
    _uiLayer->addChild(_lbScore);
    _uiLayer->addChild(_lbLength);

    scheduleUpdate();
    return true;
}

void GameScene::update(float dt)
{
    for (auto street : _streets){
        if (street->getPosition3D().z >1100) street->setPosition3D({0,0,750});
        else  street->setPosition3D(street->getPosition3D() + Vec3{0, 0, 0.33f});
    }
}