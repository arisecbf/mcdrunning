//
//  GameState.cpp
//  mcdrunning
//
//  Created by chenbingfeng on 15/6/19.
//
//

#include "GameState.h"
#include "urlencode.h"
#include "ServerConn.h"

#include "../rapidjson/document.h"
#include "../rapidjson/writer.h"
#include "../rapidjson/stringbuffer.h"

#include "format.h"

using namespace cocos2d;
using namespace cocos2d::network;

GameState GameState::_instance;
static const std::string SERVER_BASE_URL = "http://10.211.55.7:8888";

enum REQ_TYPE{
    REQ_NEWACCT,

};

void GameState::newAccount(GS_CALL_BACK callback)
{
    int tag = _cbMapIncrimentTag++;
    std::stringstream is;
    is << "tag" << tag;
    auto tagStr = is.str();
    _cbMap[tagStr] = callback;

    HttpRequest* request = new HttpRequest();
    request->setUrl((SERVER_BASE_URL + "/newAccount").c_str());

    request->setRequestType(HttpRequest::Type::POST);
    request->setResponseCallback(CC_CALLBACK_2(GameState::onHttpRequestCompleted,this));

    // gen random id_string
    auto ra = tr::Rand_int(0,10);
    std::stringstream is2;
    is2 << "isn";
    for (int i = 0; i < 32; i++) {
        is2 << ra();
    }
    _idString = is2.str();


    // write the post data
    std::string postData = fmt::sprintf("{\"id_string\":\"%s\"}", _idString);
    CCLOG("%s", postData.c_str());
    auto dec = tr::encry(postData.c_str());
    CCLOG("%s",dec.c_str());
    auto sendData = tr::UrlEncode(dec);
    CCLOG("%s", sendData.c_str());
    auto pdata = fmt::sprintf("p=%s", sendData);
    request->setRequestData(pdata.c_str(), pdata.size());

    request->setTag(tagStr.c_str());
    request->setUserData((void *)REQ_NEWACCT);
    cocos2d::network::HttpClient::getInstance()->send(request);
    request->release();
}

void GameState::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{
    if (!response)
    {
        return;
    }

    // get info
    intptr_t type = (intptr_t)(response->getHttpRequest()->getUserData());
    std::string tagStr = response->getHttpRequest()->getTag();
    auto callback = _cbMap[tagStr];
    _cbMap.erase(tagStr);


    // check
    if (response->getResponseCode() != 200 || !response->isSucceed()) {
        CCLOG("response error, status=%ld msg=%s", response->getResponseCode(), response->getErrorBuffer());
        callback(ERR_SERVERFAIL);
        return;
    }

    std::stringstream is;
    for (size_t i = 0; i < response->getResponseData()->size(); i++) {
        is << response->getResponseData()->at(i);
    }
    auto rawData = is.str();
    CCLOG("RES %s , %s", response->getResponseDataString(), rawData.c_str());

    // decode data
    auto lightData = tr::decry(rawData);
    CCLOG("RES LIGHT %s", lightData.c_str());


    rjson::Document d;
    d.Parse(lightData.c_str());

    // check inter
    assert(d.HasMember("err_code"));
    int errCode = d["err_code"].GetInt();
    if (errCode != 0) {
        CCLOG("server error code=%d, msg=%s", errCode, d["err_msg"].GetString());
        callback(ERR_SERVERFAIL);
        return;
    }

    // dispatch, save state
    switch (type) {
        case REQ_NEWACCT:
            _id = d["id"].GetInt();
            _pwd = d["pwd"].GetString();
            break;

        default:
            break;
    }

    // callback
    callback(ERR_NONE);
}