//
//  Common.h
//  mcdrunning
//
//  Created by chenbingfeng on 15/5/30.
//
//

#ifndef mcdrunning_Common_h
#define mcdrunning_Common_h

#include <string>
#include <vector>
#include <utility>

#include "cocos2d.h"

namespace config
{
    // Brand
    const float default_logo_time = 2.3f;
    const std::vector<std::pair<std::string, float> > logoImages = {
        {"images/port_i6_first_party.png", default_logo_time},
        {"images/port_i6.png", default_logo_time}
    };
    const float full_show = 0.6f;

}

namespace mcd {
}

#endif
