//
//  GameScene.h
//  mcdrunning
//
//  Created by chenbingfeng on 15/5/31.
//
//

#ifndef __mcdrunning__GameScene__
#define __mcdrunning__GameScene__

#include <vector>

#include "cocos2d.h"
#include "McdLayer.h"
#include "Common.h"

class GameScene : public cocos2d::Scene
{
public:
    virtual bool init();
    CREATE_FUNC(GameScene);

protected:
    cocos2d::Layer* _uiLayer;
    cocos2d::Layer* _gameLayer;
    cocos2d::Layer* _bgLayer;
    cocos2d::Camera* _camera;
    std::vector<cocos2d::Sprite3D* > _streets;
    std::vector<cocos2d::Sprite3D* > _jingbis;

    cocos2d::Sprite* _spBarIndex;
    cocos2d::Label* _lbLength;
    cocos2d::Label* _lbScore;
    cocos2d::Label* _lbSpeed;

    bool _beTouching = false;
    float _length; // 距离
    float _score; // 分数
    float _speed; // 速度，多少距离每帧

    float _barX;
    float _barW;
    float _barIndex = 0.f; // 控制点所在bar的位置，[0, 100]，bar总长为100.


    float _barMovePerFrameRight = 0.8f; // 在按住屏幕时，控制点往高处移动速度，按每秒30帧计算。
    float _barMovePerFrameLeft = 0.8f; //在松开屏幕时，控制点往低处移动速度，按每秒30帧计算。
    float _barSplit0 = 33.f; //减速区分界线
    float _barSplit1 = 66.f; //提速区分界线
    float _accelerationLeft = -1.f; //减速的加速度
    float _accelerationRight = 1.f; //加速的加速度
    float _speedMax = 200.f;
    float _3dMoveMax = 0.5f;
    const int _streetMax = 10;
    const float _perStreetLength = 180.f;
    const float _zNear = 1200;
    const float _zFar = _zNear - _streetMax * _perStreetLength;


    void nextFrame(); // 每个frame的计算，计算新速度，距离，分数，判断死亡？

    void update(float dt) override;
};

#endif /* defined(__mcdrunning__GameScene__) */
