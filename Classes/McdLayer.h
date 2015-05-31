//
//  McdLayer.h
//  mcdrunning
//
//  Created by chenbingfeng on 15/5/31.
//
//

#ifndef __mcdrunning__McdLayer__
#define __mcdrunning__McdLayer__

#include "cocos2d.h"
#include "Common.h"
#include "ui/CocosGUI.h"

class McdLayer : public cocos2d::Layer
{
protected:
    virtual cocos2d::ui::Button* decorateButton(const config::Btn_info& btnInfo , int tag);
    virtual cocos2d::Sprite* decorateImg(const std::string& img, const cocos2d::Vec2& pos);
    virtual void touchEvent(Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    virtual void onButtonClick(int tag) = 0;
};

#endif /* defined(__mcdrunning__McdLayer__) */
