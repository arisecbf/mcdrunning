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

    const float _SPEED_MIN = 10.f;
    float _speed = _SPEED_MIN; // Z方向移动每秒

    cocos2d::Layer* _3dGameLayer;
    cocos2d::Layer* _2dGameLayer;
    cocos2d::Layer* _3dUiLayer;
    cocos2d::Layer* _2dUiLayer;

    cocos2d::Camera* _3dCamera;

    // 道具
    int _propEnableTimeLeft[Prop::TMAX] = {0};
    std::vector<cocos2d::Sprite3D*> _props;
    const float _PROP_START_Z = 1005;
    void putPropInGame(int type);
    void checkPropClick(const cocos2d::Vec2& loc);
    void dealPickedProp(cocos2d::Sprite3D* sp);

    // 加速条
    int _isSpeeding = 0;


    void update(float dt)override;
};

#endif /* defined(__mcdrunning__RunningScene__) */
