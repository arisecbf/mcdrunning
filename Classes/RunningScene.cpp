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
#include "SimpleAudioEngine.h"

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
        auto sp3d = Sprite3D::create("3d/street_wall.c3b");
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
    _camera->setPosition3D({0, 10, 1000});
    _camera->lookAt({0,0,0});
    _camera->setCameraFlag(CameraFlag::USER1);
    _3dGameLayer->addChild(_camera);
    _3dGameLayer->setCameraMask((unsigned short)CameraFlag::USER1);
    _3dCamera = _camera;

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

    /////////////////////多点触摸////////////////////////
    // 对道具，障碍物的点击以 点下位置为准，移动无效。
    // 加速条的控制，凡是有触摸存在则为on，否则off。
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = [this](const std::vector<Touch*>& touches, Event* event){
        _isSpeeding += touches.size();
        for (int i = 0; i < touches.size(); i++) {
            auto th = touches[i];
            this->checkPropClick(th->getLocationInView());
        }
    };

    listener->onTouchesCancelled = [this](const std::vector<Touch*>& touches, Event* event){
        _isSpeeding -= touches.size();

    };

    listener->onTouchesEnded = [this](const std::vector<Touch*>& touches, Event* event){
        _isSpeeding -= touches.size();

    };

    listener->onTouchesMoved = [this](const std::vector<Touch*>& touches, Event* event){
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    addCommonBtn({.5f, .1f}, "test", [this](){this->putPropInGame(0);});
}

void RunningScene::initUIPart()
{

}

static const int street_width = 20;

void RunningScene::putPropInGame(int type)
{
    auto sp = Sprite3D::create("3d/milkx.c3b");
    sp->setPosition3D({rand()%20 - .5f * street_width,0.f,_PROP_START_Z});
    sp->setCameraMask((unsigned short)CameraFlag::USER1);
    sp->runAction(RepeatForever::create(RotateBy::create(2.f, {0,360,0})));
    _3dGameLayer->addChild(sp, 2);
    _props.push_back(sp);
}


void RunningScene::update(float dt)
{
    // 以_speed为核心的街道移动
    float moveDistance = _speed * dt;
    for (int i = 0; i < _streets.size(); i++){
        auto street = _streets[i];
        street->setPosition3D(street->getPosition3D() - Vec3{0, 0, moveDistance});
        if ( street->getPosition3D().z < _STREET_Z_FAR) {
            street->setPosition3D(street->getPosition3D() + Vec3{0, 0, _STREET_N_MAX * _STREET_PER_LENGTH});
        }
    }

    // 道具移动
    for (auto iter = _props.begin(); iter != _props.end(); ) {
        auto sp = *iter;
        sp->setPosition3D(sp->getPosition3D() - Vec3{0, 0, moveDistance});
        if (sp->getPosition3D().z < _STREET_Z_FAR) {
            _3dGameLayer->removeChild(sp);
            iter = _props.erase(iter);
        } else {
            iter++;
        }
    }

    //
}

static const float TOUCH_SCOPE = 40*40; // 触摸点中阈值
static bool checkIfTouchOn(const Vec2& a, const Vec2& b){
    auto diff = a - b;
    return TOUCH_SCOPE > diff.x * diff.x + diff.y * diff.y;
}
void RunningScene::checkPropClick(const cocos2d::Vec2& loc)
{
    CCLOG("touch loc %f %f", loc.x, loc.y);
    for (auto iter = _props.begin(); iter < _props.end();) {
        auto propLoc = _3dCamera->project((*iter)->getPosition3D());
        CCLOG("prop %f %f", propLoc.x, propLoc.y);
        auto diff = loc - propLoc;
        if (TOUCH_SCOPE > diff.x*diff.x + diff.y*diff.y) {
            // 点中了，从vec删除，然后进行后续动画。
            auto ap = *iter;
            iter = _props.erase(iter);
            dealPickedProp(ap);
        } else {
            iter++;
        }
    }
}

void RunningScene::dealPickedProp(cocos2d::Sprite3D* sp)
{
    // TODO，把类型信息放在sp的userdata里面。

    // sound
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/prop_pick.mp3");

    auto act = MoveTo::create(2.f, {0, 0, 200});
    sp->runAction(act);

    scheduleOnce([this,sp](float dt){ this->_3dGameLayer->removeChild(sp); }, 2.2f, "sdfs");
}

