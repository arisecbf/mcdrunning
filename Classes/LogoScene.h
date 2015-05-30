//
//  LogoScene.h
//  mcdrunning
//
//  Created by chenbingfeng on 15/5/30.
//
//

#ifndef __mcdrunning__LogoScene__
#define __mcdrunning__LogoScene__

#include "cocos2d.h"

class LogoScene : public cocos2d::Layer
{
public:

    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(LogoScene);
};

#endif /* defined(__mcdrunning__LogoScene__) */
