//
//  CharacterScene.cpp
//  mcdrunning
//
//  Created by chenbingfeng on 15/6/22.
//
//

#include "CharacterScene.h"
#include "StartScene.h"
#include "Msg.h"
#include "RoleSelectScene.h"
#include "GameState.h"
#include "format.h"
#include "uiconf.h"
#include "GoldLayer.h"

USING_NS_CC;

class SpGroup: public Sprite
{
public:
    bool init() override{
        assert(Sprite::init());
        return true;
    }
    CREATE_FUNC(SpGroup);

    void addNode(Node* node){
        this->addChild(node);
        _nodes.push_back(node);
    }

    void setOpacity(GLubyte opacity) override{
        Sprite::setOpacity(opacity);
        for (auto pn:_nodes){
            pn->setOpacity(opacity);
        }
    }
private:
    std::vector<Node*> _nodes;
};

bool CharacterScene::init()
{
    assert(TRBaseScene::init());

    // bg
    this->addCommonBg();

    // back btn
    this->addCommonBackBtn([](){
        Director::getInstance()->replaceScene(StartScene::createScene());
    });

    this->addChild(GoldLayer::create());

    // test btn
    _btnSelect = addCommonBtn({.5,.1}, Msg::s()["select"], [this](){this->select();});
    _btnSelect->setVisible(false);

    _btnUnlock = addCommonBtn({.5,.1}, Msg::s()["unlock"], [this](){this->unlock();});
    _btnUnlock->setVisible(false);

    addCommonBtn({.2f,.1f}, Msg::s()["prev"], [this](){this->prev();});
    addCommonBtn({.8f,.1f}, Msg::s()["next"], [this](){this->next();});

    _roleIds = GameState::s()->getCharacterIds();
    for (int i = 0; i < _roleIds.size(); i++) {
        if (GameState::s()->getSelectedCharacterId() == _roleIds[i]) {
            _roleShowIndex = i;
            break;
        }
    }

    // generate all role
    for (int id : _roleIds) {
        auto pch = GameState::s()->getCharacter(id);
        auto spHold = SpGroup::create();
        spHold->setPosition(genPos({.5f,.4f}));
        _defaultLayer->addChild(spHold, 2);
        _roleMap[id] = spHold;

        // main image
        auto spMainImage = Sprite::create(fmt::sprintf("images/roles/%d.png", id));
        spMainImage->setPosition(genPos({0.f, 0.15f}));
        spHold->addNode(spMainImage);

        // desc
        auto lbDesc = Label::createWithTTF(pch->desc, uic::font_zh, 25);
        lbDesc->setPosition(genPos({0.f, -0.2f}));
        spHold->addNode(lbDesc);

        // title
        auto lbTitle = Label::createWithTTF(pch->title, uic::font_zh, 40);
        lbTitle->setPosition(genPos({0.f, 0.43f}));
        spHold->addNode(lbTitle);

        // gold
        auto lbGold = Label::createWithTTF(fmt::sprintf("%d", pch->gold), uic::font_en, 30);
        lbGold->setColor(Color3B::GRAY);
        lbGold->setPosition(genPos({0.f, 0.4f}));
        spHold->addNode(lbGold);
        
        spHold->setOpacity(0);
    }

    roleDispUpdate();
    return true;
}

static const float ani_dt = 0.2f;
void CharacterScene::roleDispUpdate(){
    for (int i = 0; i < _roleIds.size(); i++) {
        auto sp = _roleMap[_roleIds[i]];
        Vec2 newPos = genPos({.5f + (i - _roleShowIndex) * 0.3f, 0.4f});
        sp->runAction(MoveTo::create(ani_dt, newPos));
        sp->setZOrder(100 - std::abs(i - _roleShowIndex));
        if (i == _roleShowIndex) {
            auto fi = FadeIn::create(ani_dt);
            sp->runAction(fi);
        }
        if (i == _roleShowIndexLast) {
            auto fo = FadeOut::create(ani_dt);
            sp->runAction(fo);
        }
    }
    int id = _roleIds[_roleShowIndex];
    if (GameState::s()->isIdUnlock(id)){
        _btnSelect->setVisible(true);
        _btnUnlock->setVisible(false);
    } else {
        _btnSelect->setVisible(false);
        _btnUnlock->setVisible(true);
    }
}

void CharacterScene::next(){
    _roleShowIndexLast = _roleShowIndex;
    _roleShowIndex = std::min((int)_roleIds.size() - 1, _roleShowIndex + 1);
    if (_roleShowIndex != _roleShowIndexLast) {
        roleDispUpdate();
    }
}

void CharacterScene::prev(){
    _roleShowIndexLast = _roleShowIndex;
    _roleShowIndex = std::max(0, _roleShowIndex - 1);
    if (_roleShowIndex != _roleShowIndexLast) {
        roleDispUpdate();
    }
}

void CharacterScene::select(){
    int id = _roleIds[_roleShowIndex];
    GameState::s()->setSelectedCharacterId(id);
}

void CharacterScene::unlock(){
    int id = _roleIds[_roleShowIndex];
    if (GameState::s()->unlockCharacterId(id)){
        _btnSelect->setVisible(true);
        _btnUnlock->setVisible(false);
    }
}

