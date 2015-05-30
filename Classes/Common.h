//
//  Common.h
//  mcdrunning
//
//  Created by chenbingfeng on 15/5/30.
//
//

#ifndef mcdrunning_Common_h
#define mcdrunning_Common_h

#include <string>
#include <vector>
#include <utility>

#include "cocos2d.h"

// 为了适应不同屏幕，位置信息均要求是[0,1.0]相对的。
namespace config
{
    struct Btn_info{
        std::string img;
        std::string img_p;
        cocos2d::Vec2 pos;
        Btn_info(const std::string& img0, const std::string& img1, const cocos2d::Vec2& pos){
            this->img = img0;
            this->img_p = img1;
            this->pos = pos;
        }
    };
    // Brand
    const float default_logo_time = 2.3f;
    const std::vector<std::pair<std::string, float> > logoImages = {
        {"images/port_i6_first_party.png", default_logo_time},
        {"images/port_i6.png", default_logo_time}
    };
    const float full_show = 0.6f;

    // login
    const std::string login_bg = "images/login_bg.jpg";
    const std::string bgm_default = "sound/bgm_default.mp3";
    const Btn_info login_btn_qq = {"images/login_btn_qq.png", "images/login_btn_qq_p.png", {0.5f, 0.2f}};
    const Btn_info login_btn_guest {"images/login_btn_guest.png", "images/login_btn_guest_p.png", {0.5f, 0.1f}};

}
inline cocos2d::Vec2 operator*(const cocos2d::Vec2& v2, const cocos2d::Size& size)
{
    return {v2.x * size.width, v2.y * size.height};
}
namespace mcd {
    inline void scale2size(cocos2d::Sprite* sprite, const cocos2d::Size& size)
    {
        sprite->setScaleX(size.width* 1.f/sprite->getContentSize().width);
        sprite->setScaleY(size.height*1.f/sprite->getContentSize().height);
    }

}

#endif
