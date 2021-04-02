//
// Created by boot on 3/25/21.
//

#ifndef NL_NS_H
#define NL_NS_H
#include <string>

using namespace std;
string decryptByAES(const char* data, const char* secretKey, const char* iv, int iMode);

#endif //NL_NS_H
