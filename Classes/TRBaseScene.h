//
//  TRBaseScene.h
//  mcdrunning
//
//  Created by chenbingfeng on 15/6/22.
//
// 定制的Scene基类

#ifndef __mcdrunning__TRBaseScene__
#define __mcdrunning__TRBaseScene__

#include <cocos2d.h>
#include "McdLayer.h"

class McdLayer;
class TRBaseScene:public cocos2d::Scene
{
public:
    virtual bool init() override;
    CREATE_FUNC(TRBaseScene);

    void addCommonBg();
    cocos2d::Vec2 genPos(const cocos2d::Vec2& pos);
    cocos2d::Vec3 genPos3D(const cocos2d::Vec2& pos, float z);

protected:
    McdLayer* _defaultLayer;
};

#endif /* defined(__mcdrunning__TRBaseScene__) */
