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

    // score
    int _length;
    int _score;
    cocos2d::Label* _lbLength;
    cocos2d::Label* _lbScore;

    void update(float dt) override;
};

#endif /* defined(__mcdrunning__GameScene__) */
