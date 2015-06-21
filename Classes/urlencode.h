//
//  urlencode.h
//  mcdrunning
//
//  Created by chenbingfeng on 15/6/21.
//
//

#ifndef __mcdrunning__urlencode__
#define __mcdrunning__urlencode__

#include <string>

namespace tr {

std::string UrlEncode(const std::string& str);

std::string UrlDecode(const std::string& str);

} // tr

#endif /* defined(__mcdrunning__urlencode__) */
