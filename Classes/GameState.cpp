//
//  GameState.cpp
//  mcdrunning
//
//  Created by chenbingfeng on 15/6/19.
//
//

#include "GameState.h"
#include "ServerConn.h"

#include "../rapidjson/document.h"
#include "../rapidjson/writer.h"
#include "../rapidjson/stringbuffer.h"

#include "format.h"
#include <cstdlib>
#include <ctime>

using namespace cocos2d;
using namespace cocos2d::network;

GameState GameState::_instance;
static const std::string SERVER_BASE_URL = "http://10.211.55.7:8888";

enum REQ_TYPE{
    REQ_NEWACCT,

};

GameState::GameState()
{
    std::srand((unsigned int)std::time(0));
    for (int i = 0; i < Prop::TMAX; i++) {
        _propMap[i] = Prop(i, 0);
    }
}

void GameState::load()
{
    //character data
    auto data = FileUtils::getInstance()->getDataFromFile("characters.json");
    unsigned char* pc = (unsigned char*)malloc((data.getSize() + 1) * sizeof(unsigned char));
    assert(pc != nullptr);
    for (int i = 0; i < data.getSize(); i++) {
        pc[i] = data.getBytes()[i];
    }
    pc[data.getSize()] = '\0';
    rjson::Document doc;
    doc.Parse((char*)pc);

    assert(doc.HasMember("data"));
    auto& list = doc["data"];
    assert(list.IsArray());
    for (rjson::SizeType i = 0; i < list.Size(); i++) {
        auto& ch = list[i];
        int id = ch["id"].GetInt();
        int gold = ch["gold"].GetInt();
        std::string title = ch["title"].GetString();
        std::string desc = ch["desc"].GetString();
        CCLOG("%d,%d,%s",id,gold,title.c_str());
        _characterIds.push_back(id);
        _characterMap[id] = {id, gold, title, desc};
    }

    free(pc);
    pc = nullptr;
}

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
    std::stringstream is2;
    is2 << "isn";
    for (int i = 0; i < 32; i++) {
        is2 << std::rand()%10;
    }
    _idString = is2.str();

    // write the post data
    std::string postData = fmt::sprintf("{\"id_string\":\"%s\"}", _idString);
    CCLOG("%s", postData.c_str());
    auto dec = tr::encry(postData.c_str());
    auto pdata = fmt::sprintf("p=%s", dec);
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

void GameState::store()
{
    rjson::Document doc;
    doc.SetObject();
    doc.AddMember("gold", _gold, doc.GetAllocator());
    doc.AddMember("milk", _propMap[Prop::MILK].level, doc.GetAllocator());
    doc.AddMember("magnet", _propMap[Prop::MAGNET].level, doc.GetAllocator());
    doc.AddMember("xo", _propMap[Prop::XO].level, doc.GetAllocator());
    doc.AddMember("redwine", _propMap[Prop::REDWINE].level, doc.GetAllocator());
    doc.AddMember("shield", 0, doc.GetAllocator());
    doc.AddMember("speed_length", _speedBarLength, doc.GetAllocator());
    rjson::StringBuffer buffer;
    rjson::Writer<rjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    std::string strdata = buffer.GetString();

    UserDefault::getInstance()->setStringForKey("data", strdata);
}

void GameState::recover()
{
    std::string strdata = UserDefault::getInstance()->getStringForKey("data");
    if (strdata.size() <= 0) return;

    rjson::Document doc;
    doc.Parse(strdata.c_str());
    _gold = doc["gold"].GetInt();
    _propMap[Prop::MILK].level = doc["milk"].GetInt();
    _propMap[Prop::MAGNET].level = doc["magnet"].GetInt();
    _propMap[Prop::XO].level = doc["xo"].GetInt();
    _propMap[Prop::REDWINE].level = doc["redwine"].GetInt();
    _propMap[Prop::SHIELD].level = 0;

    _speedBarLength = doc["speed_length"].GetInt();
}
