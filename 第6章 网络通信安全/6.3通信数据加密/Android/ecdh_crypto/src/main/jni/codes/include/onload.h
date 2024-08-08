//
//  Created by AYL on 19/8/23.
//

#ifndef _ONLOAD_
#define _ONLOAD_
#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// 对应的java层函数名字
#define NATIVE_AES_ENCRYPT  "m"
#define NATIVE_AES_DECRYPT "h"
#define NATIVE_CREATE_KEY "o"
#define NATIVE_CALC_SECRET "a"
#define NATIVE_ECDSA_SIGN "ecdsaSign"

// 指定要注册的类，对应完整的java类名
#define JNIREG_CLASS "com/ecdh/crypto/jni/Native"

// 导出函数名进行宏定义 
#define EXPORT_AES_ENCRYPT  _test1   
#define EXPORT_AES_DECRYPT  _test2
#define EXPORT_CREATE_KEY  _test3
#define EXPORT_CALC_SECRET   _test4
#define EXPORT_ECDSA_SIGN   _test5

// 导出函数的签名
#define METHOD_AES_ENCRYPT_SIGN  "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"
#define METHOD_AES_DECRYPT_SIGN  "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"
#define METHOD_CREATE_KEY_SIGN   "()Ljava/lang/String;"
#define METHOD_ECDSA_SIGN_SIGN   "(Ljava/lang/String;)Ljava/lang/String;"
#define METHOD_CALC_SECRET_SIGN  "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"

#endif