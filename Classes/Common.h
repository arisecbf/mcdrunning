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
        float scale;
        Btn_info(const std::string& img0, const std::string& img1, const cocos2d::Vec2& pos, float scale){
            this->img = img0;
            this->img_p = img1;
            this->pos = pos;
            this->scale = scale;
        }
    };
    // common
    const std::string white_bg = "images/white_bg.png";
    const float default_turn_in_time = 0.3f;
    const float default_turn_out_time = 0.2f;
    
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
    const Btn_info login_btn_qq = {"images/login_btn_qq.png", "images/login_btn_qq_p.png", {0.5f, 0.2f}, 1.f};
    const Btn_info login_btn_guest {"images/login_btn_guest.png", "images/login_btn_guest_p.png", {0.5f, 0.1f}, 1.f};

    // start
    const float start_scale = 2.f;
    const std::string start_bg = "images/start_bg.png";
    const Btn_info start_btn_coin = {"images/coin_bg.png", "images/coin_bg.png", {0.75f, 0.9f}, start_scale};
    const Btn_info start_btn_head = {"images/start_btn_head.png", "images/start_btn_head.png", {0.7f, 0.7f}, start_scale};
    const Btn_info start_btn_role = {"images/start_btn_role.png", "images/start_btn_role.png", {0.3f, 0.3f}, start_scale};
    const Btn_info start_btn_asset = {"images/start_btn_asset.png", "images/start_btn_asset.png", {0.5f, 0.3f}, start_scale};
    const Btn_info start_btn_drink = {"images/start_btn_drink.png", "images/start_btn_drink.png", {0.7f, 0.3f}, start_scale};
    const Btn_info start_btn_setting = {"images/start_btn_setting.png", "images/start_btn_setting.png", {0.2f, 0.2f}, start_scale};
    const Btn_info start_btn_rank = {"images/start_btn_rank.png", "images/start_btn_rank.png", {0.8f, 0.2f}, start_scale};
    const Btn_info start_btn_play = {"images/start_btn_play.png", "images/start_btn_play.png", {0.5f, 0.2f}, start_scale};
    const std::string start_role = "images/role_0.png";
    const cocos2d::Vec2 start_img_role = {0.5f, 0.5f};

    // role select
    const Btn_info role_btn_select = {"images/role_btn_select.png", "images/role_btn_select.png", {0.5f, 0.2f}, 2.f};
    const Btn_info role_btn_prev = {"images/role_btn_prev.png", "images/role_btn_prev.png", {0.2f,0.2f}, 1.f};
    const Btn_info role_btn_next = {"images/role_btn_next.png", "images/role_btn_next.png", {0.8f, 0.2f}, 1.f};
    const Btn_info btn_back = {"images/btn_back.png", "images/btn_back.png", {0.15f, 0.9f}, 2.f};
    const cocos2d::Vec2 role_pos_middle = {0.5f, 0.5f};
    const cocos2d::Vec2 role_pos_step = {0.17f, 0.f};
    const float role_scale_step = 0.8f;
    const std::vector<std::string> role_vec_imgs = {
        "images/roles/0.png",
        "images/roles/1.png",
        "images/roles/2.png",
        "images/roles/3.png",
        "images/roles/4.png",
        "images/roles/5.png"};
    const float role_img_scale = 1.f;
    const float role_interval = 0.3f;

    // game scene
    const Btn_info game_btn_back = {"images/btn_back.png", "images/btn_back.png", {0.1f, 0.94f}, 2.f};

}

inline cocos2d::Vec2 operator*(const cocos2d::Vec2& v2, const cocos2d::Size& size)
{
    return {v2.x * size.width, v2.y * size.height};
}

inline cocos2d::Vec2 operator*(const cocos2d::Size& size, const cocos2d::Vec2& v2)
{
    return v2*size;
}

namespace mcd {
    inline void scale2size(cocos2d::Sprite* sprite, const cocos2d::Size& size)
    {
        sprite->setScaleX(size.width* 1.f/sprite->getContentSize().width);
        sprite->setScaleY(size.height*1.f/sprite->getContentSize().height);
    }
}

#endif
