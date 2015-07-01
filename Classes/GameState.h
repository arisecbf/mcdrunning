//
//  GameState.h
//  mcdrunning
//
//  Created by chenbingfeng on 15/6/19.
//
//

#ifndef __mcdrunning__GameState__
#define __mcdrunning__GameState__

#include <stdio.h>
#include "cocos2d.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include "network/HttpClient.h"

class Record
{
public:
    int id;
    int score;
    Record(int i, int s):id(i),score(s){};
};

class Character
{
public:
    int id = 0;
    int gold = 0;
    std::string title = "";
    std::string desc = "";

    // 其它属性
    Character(){};

    Character(int id, int gold, const std::string& title, const std::string& desc):
        id(id), gold(gold), title(title), desc(desc){};
};

class Prop
{
public:
    enum PropType{
        REDWINE,
        XO,
        MAGNET,
        MILK,
        SHIELD,
        TMAX
    };

    int type;
    int level;
    bool canUpgrade(){
        return type != SHIELD && level < 5;
    }
    int upgradeCost(){
        return level == 0 ? 500:
        level == 1 ? 1000:
        level == 2 ? 2000:
        level == 3 ? 3500:
        level == 4 ? 5000: 0;
    }
    std::string getTitle(){
        return type == REDWINE? "redwine":
        type == XO? "xo":
        type == MAGNET? "magnet":
        type == MILK? "milk":
        type == SHIELD? "shield":">>>error<<<";
    }
    int count;
    Prop():type(0),level(0){};
    Prop(int type, int level):
        type(type), level(level){};

    float getEnableTime(){
        switch (type) {
            case REDWINE:
            case MILK:
            case XO:
                return 3.f + 1.f*level;
            case MAGNET:
                return 5.f + 2.f*level;
            default:
                return  10.f;
        }
    }
};

class GameState
{
public:
    typedef std::function<void(int)> GS_CALL_BACK;
    enum CB_ERR_CODE{
        ERR_NONE,
        ERR_NETWORK,
        ERR_SERVERFAIL,
    };

    // Singleton access
    static GameState* g(){ return &_instance; };
    static GameState* k(){ return &_instance; };
    static GameState* s(){ return &_instance; };

    // Store game state to file
    void store();

    // Recover game state from file
    void recover();

    void load();

    void newAccount(GS_CALL_BACK callback);

    void login(GS_CALL_BACK callback);

    void updateScore(GS_CALL_BACK callback);

    void getTop3(GS_CALL_BACK callback);

    void getNear6(GS_CALL_BACK callback);

    //get/set
    int getGold(){return  _gold;};
    void setGold(int gold){_gold = gold;}
    void addGold(int gold){_gold += gold;}
    std::vector<int> getCharacterIds(){return _characterIds;}
    Character* getCharacter(int id){ return &(_characterMap[id]);};
    int getSelectedCharacterId(){return _selectedCharId; };
    bool isIdUnlock(int id){
        for (auto idd : _unlockedCharacterIds) {
            if (idd == id) {
                return true;
            }
        }
        return false;
    }
    void setSelectedCharacterId(int id){_selectedCharId = id;};
    bool unlockCharacterId(int id){
        int gold = getCharacter(id)->gold;
        if (gold >= _gold) {
            return false;
        } else {
            _gold -= gold;
            _unlockedCharacterIds.push_back(id);
            return true;
        }
    }
    Prop* getProp(int type){
        assert(type < Prop::TMAX && type >= 0);
        return &_propMap[type];
    }
    int getSpeedBarLength(){return _speedBarLength;}
    void setSpeedBarLength(int len){ assert(len >= 60 && len <= 100); _speedBarLength = len;}
private:
    int _id;
    std::string _idString;
    std::string _pwd;
    std::string _tk;
    int _sn;
    int _score;
    std::vector<std::shared_ptr<Record>> _top3;
    std::vector<std::shared_ptr<Record>> _near6;

    int _cbMapIncrimentTag = 0;
    std::unordered_map<std::string, GS_CALL_BACK> _cbMap;

    GameState();
    static GameState _instance;

    void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

    // pure local data
    int _gold = 999;
    std::vector<int> _unlockedCharacterIds = {0};
    std::vector<int> _characterIds;
    std::unordered_map<int, Character> _characterMap;
    int _selectedCharId = 0;
    std::unordered_map<int, Prop> _propMap;
    int _speedBarLength = 60;
    int _historyHightestScore = 100;
};

#endif /* defined(__mcdrunning__GameState__) */
