//
//  RunningScene.cpp
//  mcdrunning
//
//  Created by chenbingfeng on 15/6/23.
//
//

#include "RunningScene.h"
#include "GameState.h"
#include "uiconf.h"
#include "format.h"
#include "SpeedLayer.h"
#include "GoldLayer.h"
#include "StartScene.h"

USING_NS_CC;

bool RunningScene::init()
{
    assert(TRBaseScene::init());

    addCommonBackBtn([](){Director::getInstance()->replaceScene(StartScene::createScene());});
    this->addChild(GoldLayer::create());
    this->addChild(SpeedLayer::create(true));

    initGamePart();
    initUIPart();

    scheduleUpdate();

    return true;
}

void RunningScene::initGamePart()
{
    auto size = Director::getInstance()->getVisibleSize();

    ///////////////////3D部分/////////////////////
    // 3D layer
    _3dGameLayer = Layer::create();
    this->addChild(_3dGameLayer, 1);

    auto jb = Sprite3D::create("3d/jingbix.c3b");
    jb->setScale(50.f);
    jb->setPosition3D({0,0,0});
    _3dGameLayer->addChild(jb, 1);

    // street
    _streets.reserve(_STREET_N_MAX);
    float zPosNow = _STREET_Z_NEAR;
    for (int i = 0; i < _STREET_N_MAX; i++) {
        auto sp3d = Sprite3D::create("3d/street_wall.c3t");
        sp3d->setPosition3D({0, 0, zPosNow});
        zPosNow -= _STREET_PER_LENGTH;
        sp3d->setScaleX(8.f);
        sp3d->setScaleY(5.f);
        sp3d->setScaleZ(5.f);
        _streets.push_back(sp3d);
        _3dGameLayer->addChild(sp3d, 1);
    }

    // camera
    auto _camera = Camera::createPerspective(60, size.width/size.height, 1, 1200);
    _camera->setPosition3D({0, 0, 1000});
    _camera->lookAt({0,0,0});
    _camera->setCameraFlag(CameraFlag::USER1);
    _3dGameLayer->addChild(_camera);
    _3dGameLayer->setCameraMask((unsigned short)CameraFlag::USER1);

    //////////////////////2D部分///////////////////////////
    _2dGameLayer = Layer::create();
    this->addChild(_2dGameLayer, 2);

    // 2d的主角及动画
    auto role2 = Sprite::create();
    role2->setPosition(genPos({0.5f,0.5f}));
    _2dGameLayer->addChild(role2);
    role2->setScale(2.f);
    auto cache = AnimationCache::getInstance();
    cache->addAnimationsWithFile("animations/animations-2.plist");
    auto animation2 = cache->getAnimation("dance_1");
    auto action2 = Animate::create(animation2);
    role2->runAction(RepeatForever::create(Sequence::create(action2, action2->reverse(), NULL)));

}

void RunningScene::initUIPart()
{

}


void RunningScene::update(float dt)
{
    // 以_speed为核心的街道移动
    float moveDistance = _speed * dt;
    for ( int i = 0; i < _streets.size(); i++){
        auto street = _streets[i];
        street->setPosition3D(street->getPosition3D() - Vec3{0, 0, moveDistance});
        if ( street->getPosition3D().z < _STREET_Z_FAR) {
            street->setPosition3D(street->getPosition3D() + Vec3{0, 0, _STREET_N_MAX * _STREET_PER_LENGTH});
        }
    }
}

