//
//  StartScene.h
//  mcdrunning
//
//  Created by chenbingfeng on 15/5/31.
//
//

#ifndef __mcdrunning__StartScene__
#define __mcdrunning__StartScene__

#include "cocos2d.h"
#include "McdLayer.h"
#include "GoldLayer.h"

class StartScene : public McdLayer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(StartScene);

protected:
    enum _BTN_TAG{
        BTN_COIN,
        BTN_HEAD,
        BTN_ROLE,
        BTN_ASSET,
        BTN_DRINK,
        BTN_PLAY,
        BTN_RANK,
        BTN_SETTING
    };
    void onButtonClick(int tag) override;

    GoldLayer* _goldLayer;
};
#endif /* defined(__mcdrunning__StartScene__) */
