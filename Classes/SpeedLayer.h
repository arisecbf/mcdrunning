//
//  SpeedLayer.h
//  mcdrunning
//
//  Created by chenbingfeng on 15/6/23.
//
//

#ifndef __mcdrunning__SpeedLayer__
#define __mcdrunning__SpeedLayer__

#include "cocos2d.h"
#include "McdLayer.h"

class SpeedLayer: public McdLayer
{
public:
    virtual bool init(bool showIndexbar);
    static SpeedLayer* create(bool showIndexBar) {
        auto ret = new SpeedLayer();
        if (ret && ret->init(showIndexBar)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    void setIndexPosition(float idex){ _indexPosition = idex;}
    float getIndexPosition(){return _indexPosition;};

private:
    float _indexPosition = 0.5f;

    cocos2d::Sprite* _mainSp;
    cocos2d::Sprite* _indexSp =nullptr;
    void updateIndexPos();
};

#endif /* defined(__mcdrunning__SpeedLayer__) */
