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

namespace tr {

    void test();

    std::string encry(std::string datastring);

    std::string decry(const std::string& b64string);

    class Rand_int {
    public:
        Rand_int(int lo, int hi) : p{lo,hi} { } // store the parameters
        int operator()() const { return r(); }
    private:
        std::uniform_int_distribution<>::param_type p;
        std::function<int(void)> r = bind(std::uniform_int_distribution<>{p}, std::default_random_engine{});
    };
} // tr

#endif /* defined(__mcdrunning__ServerConn__) */
