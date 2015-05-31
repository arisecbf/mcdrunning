//
//  McdLayer.h
//  mcdrunning
//
//  Created by chenbingfeng on 15/5/31.
//
//

#ifndef __mcdrunning__McdLayer__
#define __mcdrunning__McdLayer__

#include <vector>

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

    void withInOut(cocos2d::Node* node);
    void turnIn(float dt);
    void turnOut(float dt);
    void turnIn(){ turnIn(config::default_turn_in_time); }
    void turnOut(){ turnOut(config::default_turn_out_time); };

private:
    std::vector<cocos2d::Node*> _inoutNodes;
};

#endif /* defined(__mcdrunning__McdLayer__) */
