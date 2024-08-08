#ifndef _BASE64_H_
#define _BASE64_H_
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

namespace devicerisk
{

class Base64
{
public:
    static char* Encode(const char* data , int length);
    static char* Decode(const char* data, int length);

    // 修改后的base64
    static char* MEncode(const char* data , int length);
    static char* MDecode(const char *data, int data_len, int* out_len);
};
}

#endif
