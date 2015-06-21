//
//  base64.h
//  mcdrunning
//
//  Created by chenbingfeng on 15/6/18.
//
//

#ifndef __mcdrunning__base64__
#define __mcdrunning__base64__

#include <string>

std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);

#endif /* defined(__mcdrunning__base64__) */
