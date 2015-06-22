//
//  PropScene.h
//  mcdrunning
//
//  Created by chenbingfeng on 15/6/23.
//
//

#ifndef __mcdrunning__PropScene__
#define __mcdrunning__PropScene__

#include "TRBaseScene.h"

#include <unordered_map>

class PropScene:public TRBaseScene
{
public:
    virtual bool init() override;
    CREATE_FUNC(PropScene);

private:
    void upgrade(int type);

    std::unordered_map<int, cocos2d::Label*> _titleLbMap;
};

#endif /* defined(__mcdrunning__PropScene__) */
