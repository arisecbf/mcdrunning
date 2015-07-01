//
//  RunningScene.h
//  mcdrunning
//
//  Created by chenbingfeng on 15/6/23.
//
//

#ifndef __mcdrunning__RunningScene__
#define __mcdrunning__RunningScene__

#include "TRBaseScene.h"
#include "cocos2d.h"
#include <vector>
#include "GameState.h"
#include "SpeedLayer.h"

class Asset
{
    // 场景内物品
public:
    enum _type{
        PROP, //道具
        MONSTER, //有害物质
        GOLD, //金币
        TMAX
    };
    int type;
    int propType;
    cocos2d::Sprite3D* sprite;
    Asset(){};
    Asset(int type, int propType, cocos2d::Sprite3D* sprite):type(type), propType(propType), sprite(sprite){};
};

struct AssetInfo
{
    int assetType;
    int propType;
    bool empty = false;
};

class RunningScene: public TRBaseScene
{
public:
    virtual bool init()override;
    CREATE_FUNC(RunningScene);

private:

    void initGamePart();
    void initUIPart();

    std::vector<cocos2d::Sprite3D*> _streets;
    const float _STREET_Z_NEAR = 1200;
    const int _STREET_N_MAX = 10;
    const float _STREET_PER_LENGTH = 176.f;
    const float _STREET_Z_FAR = _STREET_Z_NEAR - _STREET_N_MAX * _STREET_PER_LENGTH;

    const float _CHECK_AREA_NEAR_Z = 950;
    const float _CHECK_AREA_FAR_Z = 900;

    const float _SPEED_MIN = 10.f;
    float _highestSpeed = _SPEED_MIN;
    float _speed = _SPEED_MIN; // Z方向移动每秒
    float _loadLength = 0; // 路程


    cocos2d::Layer* _3dGameLayer;
    cocos2d::Layer* _2dGameLayer;
    cocos2d::Layer* _3dUiLayer;
    cocos2d::Layer* _2dUiLayer;

    cocos2d::Camera* _3dCamera;

    cocos2d::Label* _lbRoad;
    cocos2d::Label* _lbSpeed;

    //场景内asset
    std::vector<Asset> _assets;
    void checkAssetClick(const cocos2d::Vec2& loc);
    void dealPickedAsset(const Asset& asset);
    void dealPickedProp(int propType);
    cocos2d::Sprite3D* genAssetSprite(int type, int propType);
    void putAssetInGame(int type, int propType);
    void randomPutAssetInGame();
    int _cntProp = 0;
    int _cntMonster = 0;
    int _cntGold = 0;
    //用于在sprite3d的userdata中标识类型
    AssetInfo _assetInfo[Asset::TMAX];
    AssetInfo _propInfo[Prop::TMAX];
    void initAssetInfo();

    // 道具
    float _propEnableTimeLeft[Prop::TMAX];
    cocos2d::Label* _lbPropTimeLeft[Prop::TMAX];
    std::vector<cocos2d::Sprite3D*> _props;
    const float _PROP_START_Z = 1005;

    // 加速条
    int _isSpeeding = 0;
    SpeedLayer* _speedLayer;

    // 动态参数
    float genSpeedIndexRightSpeed() {
        float ret = .005f * _speed;
        if (_propEnableTimeLeft[Prop::MILK] > 0.f) {
            ret *= 0.5f;
        }
        return ret;
    } // 当增加时指标右移动速度每秒
    float genSpeedIndexLeftSpeed() {
        float ret = .005f * _speed;
        if (_propEnableTimeLeft[Prop::MILK] > 0.f) {
            ret *= 0.5f;
        }
        return ret;
    }
    float genAcceleterUp() { return 1.f; } // 加速时的加速度每秒
    float genAcceleterDown() { return 1.f; }
    float genEnabelTime(int propType);

    // 放asset的控制状态
    float _assetPutInterval = 1.0f;
    float _assetPutIntervalcost = 0.f;
    float genNextPutInterval();
    AssetInfo genNextAsset();

    // 倒计时开始
    void startCountDown();
    void showCountDown(int count);
    cocos2d::Label* _lbCountDown;

    // 右侧道具栏
    cocos2d::Sprite3D* _spRightProps[Prop::TMAX];


    void update(float dt)override;
    void gameStart();
    void gameOver();
};

#endif /* defined(__mcdrunning__RunningScene__) */
