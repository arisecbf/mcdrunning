//
//  GoldLayer.h
//  mcdrunning
//
//  Created by chenbingfeng on 15/6/22.
//
//

#ifndef __mcdrunning__GoldLayer__
#define __mcdrunning__GoldLayer__

#include "cocos2d.h"
#include "McdLayer.h"

// 金币，内聚的，不依赖于GameState单例
class GoldLayer: public McdLayer
{
public:
    virtual bool init() override;
    CREATE_FUNC(GoldLayer);

    // 更新金币数量
    void updateGold(float dt);

protected:
    virtual void onButtonClick(int tag) override;
    cocos2d::Label* _lb;
    int _goldDisp = 0;
    int _goldAniFrom, _goldAniTo;
    int _goldAniIndex;
};

#endif /* defined(__mcdrunning__GoldLayer__) */
