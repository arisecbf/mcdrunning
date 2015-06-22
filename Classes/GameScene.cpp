//
//  GameScene.cpp
//  mcdrunning
//
//  Created by chenbingfeng on 15/5/31.
//
//

#include "GameScene.h"
#include "McdLayer.h"
#include "StartScene.h"
USING_NS_CC;

class GameUILayer : public McdLayer
{
public:
    virtual bool init() override{
        if (!McdLayer::init()) return false;

        decorateButton(config::game_btn_back, BTN_BACK);

        return  true;
    };

    CREATE_FUNC(GameUILayer);

protected:
    enum _BTN_TAG{
        BTN_BACK
    };
    void onButtonClick(int tag) override{
        switch(tag){
            case BTN_BACK:
                Director::getInstance()->replaceScene(StartScene::createScene());
                break;
        }
    }
};
bool GameScene::init()
{
    if (!Scene::init()){
        return false;
    }
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
    // streets
    _streets.reserve(_streetMax);
    _jingbis.reserve(_streetMax);
    float z_pos_now = _zNear;
    for (int i = 0; i < _streetMax; i++) {
        auto sp3d = Sprite3D::create("3d/street_wall.c3t");
        if (sp3d == nullptr) {CCLOG("nullptr sp3d");}
        auto jb = Sprite3D::create("3d/jingbix.c3b");
        sp3d->setPosition3D({0, 0, z_pos_now});
        jb->setPosition3D(sp3d->getPosition3D() + Vec3{0,5,0});
        _jingbis.push_back(jb);
        z_pos_now -= _perStreetLength;
        sp3d->setScale(5.f);
        _streets.push_back(sp3d);
        _gameLayer->addChild(sp3d, 1);
        _gameLayer->addChild(jb, 1);
        jb->setScale(3.f);
        jb->runAction(RepeatForever::create(RotateBy::create(2.f, {0,360,0})));
    }
    // camera
    _camera = Camera::createPerspective(60, size.width/size.height, 1, 1000);
    _camera->setPosition3D({0, 0, 1000});
    _camera->lookAt({0,0,0});
    _camera->setCameraFlag(CameraFlag::USER1);
    _gameLayer->addChild(_camera);
    _gameLayer->setCameraMask((unsigned short)CameraFlag::USER1);
    // front UI layer
    _uiLayer = Layer::create();
    this->addChild(_uiLayer, 2);
    // speed bar
    auto speedBar = Sprite::create("images/speed_bar.png");
    speedBar->setPosition(Vec2{0.5f, 0.85f} * size);
    speedBar->setScale(2.5f, 2.0f);
    _uiLayer->addChild(speedBar, 100);
    _spBarIndex = Sprite::create("images/speed_anchor.png");
    _spBarIndex->setPosition(speedBar->getPosition());
    _spBarIndex->setScale(1.f, 2.f);
    _uiLayer->addChild(_spBarIndex, 100);
    _barX = speedBar->getPositionX() - speedBar->getContentSize().width * 2.5f *0.5f;
    _barW = speedBar->getContentSize().width * 2.5f;
    // score labels
    _lbLength = Label::createWithTTF("0", "fonts/caton.ttf", 50);
    _lbScore = Label::createWithTTF("0", "fonts/caton.ttf", 50);
    _lbLength->setPosition(Vec2{0.25f, 0.9f} * size);
    _lbScore->setPosition(Vec2{0.75f, 0.9f} * size);
    _lbLength->setColor({0,0,0});
    _lbScore->setColor({0,0,0});
    _uiLayer->addChild(_lbScore);
    _uiLayer->addChild(_lbLength);
    _lbSpeed = Label::createWithTTF("0", "fonts/caton.ttf", 50);
    _lbSpeed->setPosition(Vec2{0.5f, 0.9f} * size);
    _lbSpeed->setColor({0,0,0});
    //_uiLayer->addChild(_lbSpeed);
    this->addChild(GameUILayer::create());
    // UIO
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [&](Touch* touch, Event* event){
        auto loc = touch->getLocation();
        if (loc.y > Director::getInstance()->getVisibleSize().height*0.8f) return false;
        CCLOG("start %f %f", loc.x, loc.y);
        this->_beTouching = true;
        return true;
    };
    listener->onTouchEnded = [&](Touch* touch ,Event* event){
        auto loc = touch->getLocation();
        CCLOG("ended %f, %f", loc.x, loc.y);
        this->_beTouching = false;
    };
    listener->onTouchCancelled = [&](Touch* touch, Event* event){
        auto loc = touch->getLocation();
        CCLOG("cancel %f %f", loc.x, loc.y);
        this->_beTouching = false;
    };

    // role
    auto role = Sprite::create();
    role->setPosition(center);
    _uiLayer->addChild(role);
    auto role2 = Sprite::create();
    role2->setPosition(Vec2{0.5f, 0.3f}*size);
    _uiLayer->addChild(role2);
    role2->setScale(5.f);
    auto animation = Animation::create();
    for( int i=1;i<15;i++)
    {
        char szName[100] = {0};
        sprintf(szName, "animation/a0/paoxiao_000%02d.png", i);
        animation->addSpriteFrameWithFile(szName);
    }
    animation->setDelayPerUnit(2.8f / 14.0f);
    animation->setRestoreOriginalFrame(true);

    auto action = Animate::create(animation);
    role->runAction(RepeatForever::create( Sequence::create(action, action->reverse(), NULL)));
    auto cache = AnimationCache::getInstance();
    cache->addAnimationsWithFile("animations/animations-2.plist");
    auto animation2 = cache->getAnimation("dance_1");
    auto action2 = Animate::create(animation2);
    role2->runAction(RepeatForever::create(Sequence::create(action2, action2->reverse(), NULL)));


    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    scheduleUpdate();
    return true;
}

void GameScene::update(float dt)
{
    CCLOG("up");
    if (_beTouching) {
        _barIndex += _barMovePerFrameRight;
    } else {
        _barIndex -= _barMovePerFrameLeft;
    }

    if (_barIndex < 0) _barIndex = 0;
    if (_barIndex > 100) _barIndex = 100;

    if (_barIndex < _barSplit0){
        // 减速区域
        _speed += _accelerationLeft;
        if (_speed < 0) {
            _speed = 0;
        }
    } if (_barIndex > _barSplit1){
        //加速区域
        _speed += _accelerationRight;
        //无最大速度限制？
    } else {
        //匀速区域
    }

    _length += _speed;
    float move3d = _speed / _speedMax * _3dMoveMax;


    for ( int i = 0; i < _streets.size(); i++){
        auto street = _streets[i];
        auto jb = _jingbis[i];
        street->setPosition3D(street->getPosition3D() - Vec3{0, 0, move3d});
        if ( street->getPosition3D().z < _zFar) street->setPosition3D(street->getPosition3D() + Vec3{0,0,_streetMax * _perStreetLength});
        jb->setPosition3D(street->getPosition3D()+ Vec3{0,5,0});
    }

    std::stringstream s;
    s <<"L:" << (int)(_length/20);
    _lbLength->setString(s.str());
    _lbScore->setString(s.str());
    std::stringstream s2;
    s2 << "S:" << (int)(_speed/20);
    _lbScore->setString(s2.str());
    _spBarIndex->setPositionX(_barX + _barIndex * 0.01f * _barW);
}