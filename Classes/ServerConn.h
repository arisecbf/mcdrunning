//
//  ServerConn.h
//  mcdrunning
//
//  Created by chenbingfeng on 15/6/18.
//
//

#ifndef __mcdrunning__ServerConn__
#define __mcdrunning__ServerConn__

#include "cocos2d.h"

namespace sco {

    void httpTest();
    class AESB64 {
    public:
//        void init(const std::string& key, const std::string& iv);
        std::string encry(std::string datastring);
        std::string decry(const std::string& b64string);
    };


} // sco

#endif /* defined(__mcdrunning__ServerConn__) */
