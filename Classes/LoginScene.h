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
class LoginScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init() override;

    CREATE_FUNC(LoginScene);

private:
    enum _BTN_TAG{
        BTN_QQ,
        BTN_GUEST
    };

    void decorateButton(const config::Btn_info& btnInfo , int tag);
    void touchEvent(Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};

#endif /* defined(__mcdrunning__LoginScene__) */
