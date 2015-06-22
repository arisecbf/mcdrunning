//
//  CharacterScene.h
//  mcdrunning
//
//  Created by chenbingfeng on 15/6/22.
//
// 角色选择

#ifndef __mcdrunning__CharacterScene__
#define __mcdrunning__CharacterScene__
#include "TRBaseScene.h"
#include "cocos2d.h"
#include <string>
#include <unordered_map>
#include <vector>

class CharacterScene: public TRBaseScene
{
public:
    virtual bool init() override;
    CREATE_FUNC(CharacterScene);


private:
    cocos2d::Node* _btnSelect;
    cocos2d::Node* _btnUnlock;

    std::vector<int> _roleIds;
    int _roleShowIndex = 0;
    int _roleShowIndexLast = 99;
    std::unordered_map<int, cocos2d::Sprite*> _roleMap;

    void roleDispUpdate();
    void next();
    void prev();
    void select();
    void unlock();
};

#endif /* defined(__mcdrunning__CharacterScene__) */
