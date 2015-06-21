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
#include <random>
#include <functional>
#include <ctime>

namespace tr {

    void test();

    std::string encry(std::string datastring);

    std::string decry(const std::string& b64string);
} // tr

#endif /* defined(__mcdrunning__ServerConn__) */
