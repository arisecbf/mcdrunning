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

    // Store game state to file
    void store();

    // Recover game state from file
    void recover();

    void newAccount(GS_CALL_BACK callback);

    void login(GS_CALL_BACK callback);

    void updateScore(GS_CALL_BACK callback);

    void getTop3(GS_CALL_BACK callback);

    void getNear6(GS_CALL_BACK callback);

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
};

#endif /* defined(__mcdrunning__GameState__) */
