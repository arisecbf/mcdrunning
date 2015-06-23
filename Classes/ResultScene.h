//
//  ResultScene.h
//  mcdrunning
//
//  Created by chenbingfeng on 15/6/24.
//
//

#ifndef __mcdrunning__ResultScene__
#define __mcdrunning__ResultScene__

#include "cocos2d.h"
#include "TRBaseScene.h"

class ResultScene:public TRBaseScene
{
public:
    virtual bool init(float rawRoad, int cntProp, int cntMonster, float rawHighestSpeed, int cntGold);
    static ResultScene* create(float rawRoad, int cntProp, int cntMonster, float rawHighestSpeed, int cntGold);

};

#endif /* defined(__mcdrunning__ResultScene__) */
