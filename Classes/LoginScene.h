//
//  LoginScene.h
//  mcdrunning
//
//  Created by chenbingfeng on 15/5/31.
//
//

#ifndef __mcdrunning__LoginScene__
#define __mcdrunning__LoginScene__

#include "cocos2d.h"
#include "Common.h"
#include "ui/CocosGUI.h"
#include "McdLayer.h"
class LoginScene : public McdLayer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init() override;

    CREATE_FUNC(LoginScene);

protected:
    enum _BTN_TAG{
        BTN_QQ,
        BTN_GUEST
    };
    void onButtonClick(int tag) override;
};

#endif /* defined(__mcdrunning__LoginScene__) */
