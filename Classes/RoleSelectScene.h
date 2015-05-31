//
//  RoleSelectScene.h
//  mcdrunning
//
//  Created by chenbingfeng on 15/5/31.
//
//

#ifndef __mcdrunning__RoleSelectScene__
#define __mcdrunning__RoleSelectScene__

#include <vector>

#include "cocos2d.h"
#include "McdLayer.h"
class RoleSelectScene : public McdLayer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(RoleSelectScene);

protected:
    enum _BTN_TAG{
        BTN_COIN,
        BTN_BACK,
        BTN_SELECT,
        BTN_UNLOCK,
        BTN_PREV,
        BTN_NEXT
    };
    void onButtonClick(int tag) override;

    std::vector<cocos2d::Sprite*> _roles;
    int _index = 0;
    void animate2index(int index);
};
#endif /* defined(__mcdrunning__RoleSelectScene__) */
